#include "Library/Camera/CameraInterpole.h"

#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFlag.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;
NERVE_IMPL(CameraInterpole, Deactive);
NERVE_IMPL(CameraInterpole, ActiveRotateAxisY);
NERVE_IMPL(CameraInterpole, ActiveHermiteDistanceHV);
NERVE_IMPL(CameraInterpole, ActiveHermite);
// NERVE_IMPL(CameraInterpole, ActiveBrigade);

NERVES_MAKE_STRUCT(CameraInterpole, Deactive, ActiveRotateAxisY, ActiveHermiteDistanceHV,
                   ActiveHermite);
}  // namespace

namespace al {

CameraInterpole::CameraInterpole() : NerveExecutor("カメラ補間") {
    initNerve(&NrvCameraInterpole.Deactive, 0);
}

CameraInterpole::~CameraInterpole() {}

const f32 val1 = 150.0f;
const f32 val2 = 2000.0f;
const f32 val3 = 60.0f;
const f32 val4 = 90.0f;

void CameraInterpole::start(const CameraTicket* ticket, f32 a2, int a3) {
    _48.getPos() = _118.getPos();
    _48.getAt() = _118.getAt();
    _48.getUp() = _118.getUp();
    _48.getUp().normalize();

    if (_20) {
        // FIXME
        *(((bool*)_20) + 0x98) = false;
        _20 = nullptr;
    }

    const al::CameraTicket* prevTicket = mNextTicket;
    mNextTicket = ticket;
    mPrevTicket = prevTicket;
    _1dc = a2;

    if (a3 >= 0)
        _28 = a3;
    else if (prevTicket->getPoser()->isEndInterpoleByStep()) {
        _28 = mPrevTicket->getPoser()->getEndInterpoleStep();
    } else if (ticket->getPoser()->isInterpoleByCameraDistance()) {
        f32 v25 =
            al::normalize((_118.getPos() - ticket->getPoser()->getPosition()).length(), val1, val2);
        f32 v27 = ticket->getPoser()->isInterpoleEaseOut() ? val4 : val3;
        int InterpoleStep = (int)al::lerpValue(30.0f, v27, v25);
        sead::Vector3f prevCamDir = {0.0f, 0.0f, 0.0f};
        sead::Vector3f nextCamDir = {0.0f, 0.0f, 0.0f};
        alCameraPoserFunction::calcCameraDir(&prevCamDir, mPrevTicket->getPoser());
        alCameraPoserFunction::calcCameraDir(&nextCamDir, ticket->getPoser());
        f32 v29 = sead::Mathf::abs(sead::Mathf::rad2deg(sead::Mathf::asin(prevCamDir.y)) -
                                   sead::Mathf::rad2deg(sead::Mathf::asin(nextCamDir.y)));

        s32 v30 = v29 < 0.75f ? 0 : (int)(v29 / 0.75f);

        if (InterpoleStep <= v30)
            InterpoleStep = v30;
        _28 = InterpoleStep;
    } else {
        _28 = ticket->getPoser()->getInterpoleStep();
    }

    if (_28 < 1 || _2c) {
        _2c = 0;
        al::setNerve(this, &NrvCameraInterpole.Deactive);
        return;
    }

    if (mPrevTicket) {
        if (al::isNerve(this, &NrvCameraInterpole.Deactive) &&
            !alCameraPoserFunction::isChangeTarget(mPrevTicket->getPoser()) &&
            alCameraPoserFunction::isTargetEnableEndAfterInterpole(mPrevTicket->getPoser()) &&
            !alCameraPoserFunction::isInvalidPreCameraEndAfterInterpole(ticket->getPoser())) {
            if (mPrevTicket->getPoser() != ticket->getPoser()) {
                _20 = mPrevTicket->getPoser();
                *(((bool*)_20) + 0x98) = true;
            }
        }

        sead::Vector3f x =
            mPrevTicket->getPoser()->getPosition() - mPrevTicket->getPoser()->getTargetTrans();
        sead::Vector3f v45 =
            ticket->getPoser()->getPosition() - ticket->getPoser()->getTargetTrans();
        x.y = 0.0;
        v45.y = 0.0;
        if (al::tryNormalizeOrZero(&x) && al::tryNormalizeOrZero(&v45)) {
            f32 v41 = al::calcAngleDegree(x, v45);
            if (v41 > 105.0f) {
                if (!ticket->getPoser()->isInterpoleByCameraDistance())
                    _28 = (int)al::lerpValue(60.0f, 90.0f, al::normalize(v41, 105.0f, 180.0f));

                al::setNerve(this, &NrvCameraInterpole.ActiveRotateAxisY);
                return;
            }
        }
    }

    if (al::isNear(_3c_at, mNextTicket->getPoser()->getTargetTrans(), 250.0f) ||
        (mNextTicket->getPoser()->getTargetTrans() - _3c_at)
                .dot(mNextTicket->getPoser()->getPosition() - _30_pos) >= 0.0f)
        al::setNerve(this, &NrvCameraInterpole.ActiveHermiteDistanceHV);

    else
        al::setNerve(this, &NrvCameraInterpole.ActiveHermite);
}

void CameraInterpole::update(const sead::LookAtCamera& camera) {
    _178.getPos() = camera.getPos();
    _178.getAt() = camera.getAt();
    _178.getUp() = camera.getUp();
    _178.getUp().normalize();

    if (_2c) {
        _2c = false;
        if (isActive())
            al::setNerve(this, &NrvCameraInterpole.Deactive);
    }

    updateNerve();
    if (isActive() && al::isNear(_30_pos, _3c_at, 0.001f)) {
        _30_pos.set(_178.getPos());
        _3c_at.set(_178.getAt());
        _49_up.set(_178.getUp());
        al::setNerve(this, &NrvCameraInterpole.Deactive);
    }

    if (isActive() && mNextTicket) {
        if ((mPrevTicket && !alCameraPoserFunction::isInvalidCollider(mPrevTicket->getPoser())) ||
            !alCameraPoserFunction::isInvalidCollider(mNextTicket->getPoser()) ||
            mNextTicket->getPoser()->getPoserFlag()->_C) {
            sead::Vector3f v11v = _30_pos - _3c_at;
            float v14 = v11v.length();
            sead::Vector3f a2 = v11v;
            al::normalize(&a2);
            sead::Vector3f a3 = _49_up;
            al::verticalizeVec(&a3, a2, a3);
            if (al::tryNormalizeOrZero(&a3)) {
                sead::Vector2f v54[5];
                /* = {
                    sead::Vector2f::zero,
                    {-((75.0f / (v14 * sead::Mathf::pi2())) * 360.0f), 0.0f},
                    {(75.0f / (v14 * sead::Mathf::pi2())) * 360.0f, 0.0f},
                    {0.0f, -(15.0f / (v14 * sead::Mathf::pi2())) * 360.0f},
                    {0.0f, (15.0f / (v14 * sead::Mathf::pi2())) * 360.0f},
                };*/
                f32 p15 = (15.0f / (v14 * sead::Mathf::pi2())) * 360.0f;
                f32 p75 = (75.0f / (v14 * sead::Mathf::pi2())) * 360.0f;

                sead::Vector3f a1;
                a1.setCross(a3, a2);
                al::normalize(&a1);


                // NON_MATCHING: order of loading/storing values
                v54[0] = sead::Vector2f::zero;
                v54[1] = {-p75, 0.0f};
                v54[2] = {p75, 0.0f};
                v54[3] = {0.0f, -p15};
                v54[4] = {0.0f, p15};

                bool v16 = false;
                for (u64 v15 = 0; v15 != 5; v15++) {
                    sead::Vector3f v53 = v11v;
                    al::rotateVectorDegree(&v53, v53, a3, v54[v15].x);
                    al::rotateVectorDegree(&v53, v53, a1, v54[v15].y);
                    alCameraPoserFunction::CameraCollisionHitResult v52;
                    memset(&v52, 0, 24);
                    v52.mCollisionLocation = 3;

                    if (alCameraPoserFunction::checkFirstCameraCollisionArrow(
                            &v52, mNextTicket->getPoser(), _3c_at, v53) &&
                        !v52.mCollisionLocation) {
                        sead::Vector3f v51 = v52.mCollisionHitPos - _30_pos;
                        sead::Vector3f v50 = {0.0f, 0.0f, 0.0f};
                        if (!al::isNearZero(v51, 0.001f)) {
                            // NON_MATCHING: somehow stores an unused boolean pair per result
                            // first: false;false
                            // second: false;true
                            // third: true;true
                            if (alCameraPoserFunction::checkFirstCameraCollisionArrow(
                                    &v50, 0LL, mNextTicket->getPoser(), _30_pos, v51) ||
                                alCameraPoserFunction::checkFirstCameraCollisionArrow(
                                    &v50, 0LL, mNextTicket->getPoser(), a1 * 10.0f + _30_pos,
                                    v51) ||
                                alCameraPoserFunction::checkFirstCameraCollisionArrow(
                                    &v50, 0LL, mNextTicket->getPoser(), _30_pos - a1 * 10.0f,
                                    v51)) {
                                if ((_30_pos - v50).length() < 75.0f) {
                                } else {
                                    continue;
                                }
                            }
                        }

                        sead::Vector3f v47 = v51;
                        float v33 = v51.length();
                        float v34 = v47.length();
                        if (v34 > 0.0)
                            v47 *= (v33 + 75.0f) / v34;

                        al::parallelizeVec(&v47, a2, v47);
                        float v36 = v47.length();
                        if (_1f0 < v36) {
                            v16 = true;
                            _1f4 = 0.0f;
                            _1f0 = sead::Mathf::min(v36, (v14 + -100.0f));
                        }
                    }
                }
                if (v16) {
                    makeLookAtCamera(&_118);
                    return;
                }
            }
        }
    }

    float v37 = al::lerpValue(_1f4, 1.0f, 0.3f);
    _1f4 = al::lerpValue(_1f4, v37, 0.3f);
    // NON_MATCHING: order of loads/argument setup
    f32 v38 = al::lerpValue(_1f0, 0.0f, _1f4 * 0.3f);
    _1f0 = al::lerpValue(_1f0, v38, _1f4 * 0.3f);
    float v44 = (_30_pos - _3c_at).length();
    if ((v44 + -100.0f) < _1f0) {
        _1f0 = sead::Mathf::clampMax(_1f0, sead::Mathf::max(v44, (v44 + -100.0f)));
    }

    makeLookAtCamera(&_118);
}

bool CameraInterpole::isActive() const {
    return !isNerve(this, &NrvCameraInterpole.Deactive);
}

void CameraInterpole::makeLookAtCamera(sead::LookAtCamera* camera) const {
    sead::Vector3f v5v = {0.0f, 0.0f, 0.0f};

    if (isActive() && _1f0 > 0.1f) {
        v5v.set(_3c_at - _30_pos);
        f32 v8 = v5v.length();
        f32 f10 = _1f0;
        f32 v9 = v5v.length();
        if (v9 > 0.0f)
            v5v *= sead::Mathf::min(v8 + -100.0f, f10) / v9;
    }

    camera->getPos() = _30_pos + v5v;
    camera->getAt() = _3c_at;
    camera->getUp() = _49_up;
    camera->getUp().normalize();
}

void CameraInterpole::lerpFovyDegree(f32 a2) {
    _1d8 = lerpValue(_1dc, mNextTicket->getPoser()->getFovyDegree(), a2);
}

void CameraInterpole::exeDeactive() {
    // TODO
}

void CameraInterpole::exeActiveHermite() {
    // TODO
}

void CameraInterpole::exeActiveHermiteDistanceHV() {
    // TODO
}

void CameraInterpole::exeActiveRotateAxisY() {
    // TODO
}

void CameraInterpole::exeActiveBrigade() {
    // TODO
}

}  // namespace al
