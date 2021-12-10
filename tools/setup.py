#!/usr/bin/env python3

import argparse
import hashlib
from pathlib import Path
import subprocess
from typing import Optional
from common import setup_common as setup
from enum import Enum
import tempfile
import urllib.request

class Version(Enum):
    #VER_NAME = VER_ID, COMPRESSED_HASH, UNCOMPRESSED_HASH
    VER_100 = ["1.0",   "e21692d90f8fd2def2d2d22d983d62ac81df3b8b3c762d1f2dca9d9ab7b3053a", "18ece865061704d551fe456e0600c604c26345ecb38dcbe328a24d5734b3b4eb"]
    VER_101 = ["1.0.1", "c8531a7a14b19bc7df4738b230a7cc85d8b270264cb22bfd0dfcf3bef6fde2b6", "e4f73a2721288a93bf6038094002ea00075068276bdd814d628650227fd7d5c1"]
    VER_110 = ["1.1",   "d3afba4ee684672fac264444bfe26e908d3b5d41059d362343116792fe69a7f6", "d2f664ef1a6734803d239c56549e00b74aa534c950cdcacb605939372b7e01f3"]
    VER_120 = ["1.2",   "6d22647134b241641bf4df5e596bde48fc0ad81edf1856ffbbdf3f49b71d1219", "00c7275f67183d0a7c46c6d34349ac8988f941d8ed4162c9ce156469b3d6e4de"]
    VER_130 = ["1.3",   "47ac74a66234bc1eb717b18e01fe7c880d2dd50cf1115fd046f5e5878be5f85f", "a991fcfe32506ad42f1e425f5d3b4176bf4f95bf2a1cccec70eb933ccfe1f416"]

def check_executable(version: Version):
    path = setup.ROOT / "data" / version.value[0]
    nso = path / "main.nso"
    elf = path / "main.elf"
    return nso.is_file() and hashlib.sha256(nso.read_bytes()).hexdigest() == version.value[2] and elf.is_file()

def download_patch(v0: Version, v1: Version, temp_dir:Path) -> Path:
    print(f">>>> downloading patch from version {v0.name} to {v1.name}...")
    patch_name = v0.value[0]+"-"+v1.value[0]+".patch"
    path = temp_dir / patch_name
    urllib.request.urlretrieve("https://raw.githubusercontent.com/wiki/MonsterDruide1/OdysseyDecomp/patches/"+patch_name, path)
    return path


def prepare_single_executable(comp_v100_nso: Path, version: Version, temp_dir: tempfile.TemporaryDirectory):
    TARGET_HASH = version.value[2]
    data_path = setup.ROOT / "data" / version.value[0]
    data_path.mkdir(parents=True, exist_ok=True)
    TARGET_PATH = data_path / "main.nso"
    TARGET_ELF_PATH = data_path / "main.elf"

    if hashlib.sha256(comp_v100_nso.read_bytes()).hexdigest() != Version.VER_100.value[1]:
        setup.fail("internal error while preparing executable (not v1.0); please report")

    comp_nso = temp_dir / (version.name+"_comp.nso")
    if version != Version.VER_100:
        patch_path = download_patch(Version.VER_100, version, temp_dir)
        setup._apply_xdelta3_patch(comp_v100_nso, patch_path, comp_nso)
    else:
        comp_nso = comp_v100_nso
    
    setup._decompress_nso(comp_nso, TARGET_PATH)

    if not TARGET_PATH.is_file():
        setup.fail("internal error while preparing executable (missing NSO); please report")
    if hashlib.sha256(TARGET_PATH.read_bytes()).hexdigest() != TARGET_HASH:
        setup.fail("internal error while preparing executable (wrong NSO hash); please report")

    setup._convert_nso_to_elf(TARGET_PATH)

    if not TARGET_ELF_PATH.is_file():
        setup.fail("internal error while preparing executable (missing ELF); please report")
    

def prepare_executable(original_nso: Optional[Path]):
    temp_dir_ = tempfile.TemporaryDirectory()
    temp_dir = Path(temp_dir_.name)

    if(original_nso is not None):
        orig_hash = hashlib.sha256(original_nso.read_bytes()).hexdigest()
        v1_nso = temp_dir / "1.0.nso"
        if(orig_hash == Version.VER_130.value[1]):
            patch_path = download_patch(Version.VER_130, Version.VER_100, temp_dir)
            setup._apply_xdelta3_patch(original_nso, patch_path, v1_nso)
        elif(orig_hash == Version.VER_100.value[1]):
            v1_nso = original_nso
        else:
            setup.fail(f"unknown executable: {orig_hash}")


    for ver in Version:
        if(check_executable(ver)):
            print(f">>> {ver} is already set up")
            continue

        if original_nso is None:
            setup.fail("please pass a path to the NSO (refer to the readme for more details)")
        if not original_nso.is_file():
            setup.fail(f"{original_nso} is not a file")
        if not v1_nso.is_file():
            setup.fail(f"Failed to create 1.0-nso: {v1_nso} is not a file")
        
        prepare_single_executable(v1_nso, ver, temp_dir)

    temp_dir_.cleanup()

def create_build_dir(ver):
    build_dir = setup.ROOT / "build" / ver.value[0]
    if build_dir.is_dir():
        print(">>> build directory already exists: nothing to do")
        return

    subprocess.check_call(
        ("cmake -GNinja -DCMAKE_CXX_FLAGS=-D"+ver.name+" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_TOOLCHAIN_FILE=toolchain/ToolchainNX64.cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -B "+str(build_dir)).split(" "))
    print(">>> created build directory")

def create_build_dirs():
    for ver in Version:
        create_build_dir(ver)

def main():
    parser = argparse.ArgumentParser(
        "setup.py", description="Set up the Super Mario Odyssey decompilation project")
    parser.add_argument("original_nso", type=Path,
                        help="Path to the original NSO (1.0, compressed or not)", nargs="?")
    args = parser.parse_args()

    setup.install_viking()
    prepare_executable(args.original_nso)
    setup.set_up_compiler("4.0.1")
    print("Please download and extract clang-3.9.1 manually, until it has been properly added to the setup chain.")
    create_build_dirs()


if __name__ == "__main__":
    main()
