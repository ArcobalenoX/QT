#ifndef L2DDEFINE_H
#define L2DDEFINE_H

#include <QColor>
#include <Source/Common.h>

static const char* MOTION_GROUP_IDLE     = "idle";
static const char* MOTION_GROUP_TAP_BODY = "tap_body";

static const char* HIT_AREA_HEAD = "head";
static const char* HIT_AREA_BODY = "body";

static const char* PARAM_ANGLE_X = "PARAM_ANGLE_X";
static const char* PARAM_ANGLE_Y = "PARAM_ANGLE_Y";
static const char* PARAM_ANGLE_Z = "PARAM_ANGLE_Z";

static const char* PARAM_EYE_BALL_X = "PARAM_EYE_BALL_X";
static const char* PARAM_EYE_BALL_Y = "PARAM_EYE_BALL_Y";

static const char* PARAM_BODY_ANGLE_X = "PARAM_BODY_ANGLE_X";
static const char* PARAM_BREATH = "PARAM_BREATH";

/* 动作优先级 */
static const int PRIORITY_NONE   = 0;
static const int PRIORITY_IDLE   = 1;
static const int PRIORITY_NORMAL = 2;
static const int PRIORITY_FORCE  = 3;

class L2DDefine
{
public:
#ifdef DEVELOP
    static const bool DEBUG_LOG = false;
#else
    static const bool DEBUG_LOG = true;
#endif
};




#endif // L2DDEFINE_H
