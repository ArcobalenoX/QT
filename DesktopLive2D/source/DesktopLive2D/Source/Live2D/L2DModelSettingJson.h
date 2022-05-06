#ifndef L2D_MODEL_H
#define L2D_MODEL_H

#include "../External/Live2D/util/Json.h"

static const char NAME[]="name";
static const char MODEL[]="model";
static const char _ID[]="id";
static const char TEXTURES[]="textures";
static const char INIT_PARAM[]="init_param";
static const char INIT_PARTS_VISIBLE[]="init_parts_visible";
static const char HIT_AREAS[]="hit_areas";
static const char EXPRESSIONS[]="expressions";
static const char POSE[]="pose";
static const char PHYSICS[]="physics";
static const char MOTION_GROUPS[]="motions";
static const char SOUND[]="sound";
static const char FADE_IN[]="fade_in";
static const char FADE_OUT[]="fade_out";
static const char VAL[]="val";
static const char LAYOUT[]="layout";
static const char _FILE[]="file";

class L2DModelSettingJson
{
private:
    live2d::Json* json;

    bool ExistModelName()       {return ! json->getRoot()[NAME].isNull() ;              }
    bool ExistModelFile()       {return ! json->getRoot()[MODEL].isNull() ;             }
    bool ExistTextureFiles()    {return ! json->getRoot()[TEXTURES].isNull() ;          }
    bool ExistInitParam()       {return ! json->getRoot()[INIT_PARAM].isNull() ;        }
    bool ExistInitPartsVisible(){return ! json->getRoot()[INIT_PARTS_VISIBLE].isNull(); }
    bool ExistHitAreas()        {return ! json->getRoot()[HIT_AREAS].isNull() ;         }
    bool ExistPhysicsFile()     {return ! json->getRoot()[PHYSICS].isNull() ;           }
    bool ExistPoseFile()        {return ! json->getRoot()[POSE].isNull() ;              }
    bool ExistExpressionFile()  {return ! json->getRoot()[EXPRESSIONS].isNull() ;       }
    bool ExistMotionGroups()    {return ! json->getRoot()[MOTION_GROUPS].isNull() ;     }
    bool ExistMotionGroup(const char* name)         {return ! json->getRoot()[MOTION_GROUPS][name].isNull() ;               }
    bool ExistMotionSound(const char* name,int n)   {return ! json->getRoot()[MOTION_GROUPS][name][n][SOUND].isNull();      }
    bool ExistMotionFadeIn(const char* name,int n)  {return ! json->getRoot()[MOTION_GROUPS][name][n][FADE_IN].isNull();    }
    bool ExistMotionFadeOut(const char* name,int n) {return ! json->getRoot()[MOTION_GROUPS][name][n][FADE_OUT].isNull();   }

public:
    L2DModelSettingJson(const char* buf, int size)
    {
        json = live2d::Json::parseFromBytes(buf, size) ;
    }

    ~L2DModelSettingJson()
    {
        delete json;
    }

    const char* GetModelName()
    {
        if(!ExistModelName()) return "";
        return json->getRoot()[NAME].toString().c_str();
    }


    const char* GetModelFile()
    {
        if(!ExistModelFile()) return "";
        return json->getRoot()[MODEL].toString().c_str();
    }


    int GetTextureNum()
    {
        if(!ExistTextureFiles()) return 0;
        return json->getRoot()[TEXTURES].size() ;
    }

    const char* GetTextureFile(int n)
    {
        return json->getRoot()[TEXTURES][n].toString().c_str();
    }

    int GetInitParamNum()
    {
        if(!ExistInitParam()) return 0;
        return json->getRoot()[INIT_PARAM].size();
    }

    float GetInitParamValue(int n)
    {
        return (float)json->getRoot()[INIT_PARAM][n][VAL].toDouble();
    }

    const char* GetInitParamID(int n)
    {
        return json->getRoot()[INIT_PARAM][n][_ID].toString().c_str();
    }

    int GetInitPartsVisibleNum()
    {
        if(!ExistInitPartsVisible()) return 0;
        return json->getRoot()[INIT_PARTS_VISIBLE].size();
    }

    float GetInitPartsVisibleValue(int n)
    {
        return (float)json->getRoot()[INIT_PARTS_VISIBLE][n][VAL].toDouble();
    }

    const char* GetInitPartsVisibleID(int n)
    {
        return json->getRoot()[INIT_PARTS_VISIBLE][n][_ID].toString().c_str();
    }

    int GetHitAreasNum()
    {
        if(!ExistHitAreas()) return 0;
        return json->getRoot()[HIT_AREAS].size() ;
    }

    const char* GetHitAreaID(int n)
    {
        return json->getRoot()[HIT_AREAS][n][_ID].toString().c_str();
    }

    const char* GetHitAreaName(int n)
    {
        return json->getRoot()[HIT_AREAS][n][NAME].toString().c_str();
    }

    const char* GetPhysicsFile()
    {
        if(!ExistPhysicsFile()) return "";
        return json->getRoot()[PHYSICS].toString().c_str();
    }

    const char* GetPoseFile()
    {
        if(!ExistPoseFile()) return "";
        return json->getRoot()[POSE].toString().c_str();
    }

    int GetExpressionNum()
    {
        if(!ExistExpressionFile()) return 0;
        return json->getRoot()[EXPRESSIONS].size();
    }

    const char* GetExpressionFile(int n)
    {
        return json->getRoot()[EXPRESSIONS][n][_FILE].toString().c_str();
    }

    const char* GetExpressionName(int n)
    {
        return json->getRoot()[EXPRESSIONS][n][NAME].toString().c_str();
    }

    int GetMotionNum(const char* name)
    {
        if(!ExistMotionGroup(name))return 0;
        return json->getRoot()[MOTION_GROUPS][name].size();
    }

    const char* GetMotionFile(const char* name,int n)
    {
        if(!ExistMotionGroup(name)) return "";
        return json->getRoot()[MOTION_GROUPS][name][n][_FILE].toString().c_str();
    }


    const char* GetMotionSound(const char* name,int n)
    {
        if(!ExistMotionSound(name,n)) return "";
        return json->getRoot()[MOTION_GROUPS][name][n][SOUND].toString().c_str();
    }


    int GetMotionFadeIn(const char* name,int n)
    {
        if(!ExistMotionFadeIn(name,n)) return 1000;
        return json->getRoot()[MOTION_GROUPS][name][n][FADE_IN].toInt();
    }


    int GetMotionFadeOut(const char* name,int n)
    {
        if(!ExistMotionFadeOut(name,n)) return 1000;
        return json->getRoot()[MOTION_GROUPS][name][n][FADE_OUT].toInt();
    }


    int GetMotionGroupNum()
    {
        if (!ExistMotionGroups()) return 0;
        return json->getRoot()[MOTION_GROUPS].getKeys().size();
    }

    const char* GetMotionGroupName(int n)
    {
        if (!ExistMotionGroups())  return NULL;
        return json->getRoot()[MOTION_GROUPS].getKeys()[n].c_str();
    }

    bool GetLayout(std::map<std::string, float> & layout)
    {
        live2d::LDMap<live2d::LDString, live2d::Value* > * map=json->getRoot()[LAYOUT].getMap();
        if (map == NULL) {
            return false;
        }
        live2d::LDMap<live2d::LDString, live2d::Value* >::const_iterator map_ite;
        bool ret = false;
        for(map_ite = map->begin(); map_ite != map->end(); map_ite++)
        {
            layout[(*map_ite).first.c_str()] = (float)(*map_ite).second->toDouble();
            ret = true;
        }
        return ret;
    }
};

#endif
