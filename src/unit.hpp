#pragma once
#include <sc2api/sc2_api.h>
#include <map>
#include "unitpriority.hpp"
#include <set>

using namespace sc2;
using namespace std;

class UnitWrapper {
private:
    Composition c;
    
    UnitTypeID type;
    UnitTypeID actualType;

public:
    float health;
    float healthMax;
    float shields;
    float shieldsMax;

    Tag self;
    Point3D lastPos;
    float radius;
    bool isBuilding;
    Unit::Alliance team;
    AvailableAbilities abilities;
    int8_t ignoreFrames;

    bool hallucination;

    //UnitWrapper(Tag self_);

    //UnitWrapper(Tag self_, UnitTypeID id);

    UnitWrapper(const Unit* unit);

    bool equals(UnitWrapper *wrapper);

    inline bool exists(Agent *agent);

    inline const Unit *get(Agent *agent);

    UnitTypeID getType(Agent* agent) {
        const Unit* selfUnit = get(agent);
        if (selfUnit != nullptr) {
            actualType = selfUnit->unit_type;
        }
        return actualType;
    }

    UnitTypeID getStorageType() {
        return type;
    }

    Composition getComposition(Agent* agent) {
        const Unit* selfUnit = get(agent);
        if (selfUnit == nullptr) {
            return c;
        }
        if (selfUnit->unit_type == UNIT_TYPEID::PROTOSS_COLOSSUS) {
            return Composition::Any;
        }
        if (selfUnit->is_flying)
            return Composition::Air;
        return Composition::Ground;
    }

    void loadHealth(Agent* agent) {
        const Unit* selfUnit = get(agent);
        if (selfUnit != nullptr) {
            health = selfUnit->health;
            healthMax = selfUnit->health_max;
            shields = selfUnit->shield;
            shieldsMax = selfUnit->shield_max;
        }
    }

    Point2D pos(Agent *agent);
    Point3D pos3D(Agent *agent);

    virtual bool execute(Agent *agent);

    virtual bool executeDamaged(Agent *agent, float u_health, float u_shields) {
        return false;
    }

    static void loadAbilities(Agent *agent);

    static void loadAbilitiesEnemy(Agent* agent);

    bool checkAbility(AbilityID ability) {
        for (int i = 0; i < abilities.abilities.size(); i++) {
            if (ability == abilities.abilities[i].ability_id) {
                return true;
            }
        }
        return false;
    }

    bool operator==(const UnitWrapper &u) {
        return u.self == self;
    }

    bool operator<(const UnitWrapper &u) {
        return u.self < self;
    }

    virtual ~UnitWrapper();
};

class UnitWrapperHash {
public:
    // We use predefined hash functions of strings
    // and define our hash function as XOR of the
    // hash values.
    size_t operator()(const UnitWrapper &p) const {
        return p.self;
    }
};

using UnitWrappers = vector<UnitWrapper*>;
using UnitWrapperSet = set<UnitWrapper*>;

struct Damage {
    uint16_t normal;
    uint16_t armored;
    uint16_t light;
    uint16_t biological;
    uint16_t mechanical;
    uint16_t massive;
    uint16_t psionic;

    Damage() {
        normal = 0;
        armored = 0;
        light = 0;
        biological = 0;
        mechanical = 0;
        massive = 0;
        psionic = 0;
    }

    Damage(uint16_t normal,
    uint16_t armored,
    uint16_t light,
    uint16_t biological,
    uint16_t mechanical,
    uint16_t massive,
    uint16_t psionic) : normal(normal), armored(armored), light(light), biological(biological), mechanical(mechanical), massive(massive), psionic(psionic){
    }

    void operator+=(const Damage& u) {
        normal += u.normal;
        armored += u.armored;
        light += u.light;
        biological += u.biological;
        mechanical += u.mechanical;
        massive += u.massive;
        psionic += u.psionic;
    }

    Damage operator+(const Damage& u) {
        return Damage{
            (uint16_t)(normal + u.normal),
            (uint16_t)(armored + u.armored),
            (uint16_t)(light + u.light),
            (uint16_t)(biological + u.biological),
            (uint16_t)(mechanical + u.mechanical),
            (uint16_t)(massive + u.massive),
            (uint16_t)(psionic + u.psionic) };
    }

    Damage operator/(const float& u) {
        return Damage{
            (uint16_t)(normal / u),
            (uint16_t)(armored / u),
            (uint16_t)(light / u),
            (uint16_t)(biological / u),
            (uint16_t)(mechanical / u),
            (uint16_t)(massive / u),
            (uint16_t)(psionic / u) };
    }

    void updateHighest(Damage dmag) {
        if (dmag.normal > normal) {
            normal = dmag.normal;
        }
        if (dmag.armored > armored) {
            armored = dmag.armored;
        }
        if (dmag.light > light) {
            light = dmag.light;
        }
        if (dmag.biological > biological) {
            biological = dmag.biological;
        }
        if (dmag.mechanical > mechanical) {
            mechanical = dmag.mechanical;
        }
        if (dmag.massive > massive) {
            massive = dmag.massive;
        }
        if (dmag.psionic > psionic) {
            psionic = dmag.psionic;
        }
    }
};

struct DamageLocation {
    Damage ground;
    
    Damage air;

    void operator+=(const DamageLocation& u) {
        ground += u.ground;
        air += u.air;
    }

    DamageLocation operator+(const DamageLocation& u) {
        return DamageLocation{
            (ground + u.ground),
            (air + u.air) 
        };
    }

    DamageLocation operator/(const float& u) {
        return DamageLocation{
            (ground / u),
            (air / u)
        };
    }

    void updateHighest(DamageLocation dmagloc) {
        ground.updateHighest(dmagloc.ground);
        air.updateHighest(dmagloc.air);
    }
};

namespace UnitManager {
    map<UnitTypeID, UnitWrappers> units;
    map<UnitTypeID, UnitWrappers> neutrals;
    map<UnitTypeID, UnitWrappers> enemies;

    constexpr int damageNetPrecision = 3;
    #define blockSize 1.0F/UnitManager::damageNetPrecision

    map2d<int8_t>* enemyDamageNetReal;
    map2d<DamageLocation>* enemyDamageNet;
    map2d<int8_t>* enemyDamageNetModify;
    map2d<float>* enemyDamageNetTemp;

    //map2d<int16_t>* enemyDamageNetGround;
    //map2d<int16_t>* enemyDamageNetGroundLight;
    //map2d<int16_t>* enemyDamageNetGroundArmored;
    //map2d<int16_t>* enemyDamageNetAir;
    //map2d<int16_t>* enemyDamageNetAirLight;
    //map2d<int16_t>* enemyDamageNetAirArmored;

    //#define damageNetEnemy(p) imRef(UnitManager::enemyDamageNet, int(p.x * UnitManager::damageNetPrecision), int(p.y * UnitManager::damageNetPrecision))

    DamageLocation damageNetEnemy(Point2D p) {
        int x = int(p.x * damageNetPrecision);
        int y = int(p.y * damageNetPrecision);
        if (imRef(enemyDamageNetReal, x, y)) {
            return imRef(enemyDamageNet, x, y);
        }
        return DamageLocation{};
    }

    DamageLocation getDamageNetEnemyUnscaled(int x, int y) {
        if (imRef(enemyDamageNetReal, x, y)) {
            return imRef(enemyDamageNet, x, y);
        }
        return DamageLocation{};
    }

    void setDamageNetEnemyUnscaled(int x, int y, DamageLocation d) {
        imRef(enemyDamageNet, x, y) = d;
        imRef(enemyDamageNetReal, x, y) = 1;
    }
    
    void setEnemyDamageRadius2(Point2D pos, float radius, DamageLocation damage) {
        int x = (int)((pos.x - radius) * damageNetPrecision);
        int y = (int)((pos.y - radius) * damageNetPrecision);
        int xmax = (int)((pos.x + radius) * damageNetPrecision + 1);
        int ymax = (int)((pos.y + radius) * damageNetPrecision + 1);
        //printf("%d - %d, %d - %d\n", x, xmax, y, ymax);
        for (int i = x; i <= xmax; i++) {
            for (int j = y; j <= ymax; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                bool activate = false;
                for (float a = 0; a <= 1; a += 0.25) {
                    for (float b = 0; b <= 1; b += 0.25) {
                        Point2D temp{ i + a, j + b };
                        if (Distance2D(pos * damageNetPrecision, temp) < radius * damageNetPrecision) {
                            activate = true;
                            break;
                        }
                    }
                    if (activate) {
                        break;
                    }
                }
                if (activate) {
                    if (i > 1 && j > 1) {
                        //DamageLocation d = getDamageNetEnemyUnscaled( i - 1, j - 1);
                        setDamageNetEnemyUnscaled(i - 1, j - 1, getDamageNetEnemyUnscaled(i - 1, j - 1) + damage);
                        //d = imRef(enemyDamageNet, i - 1, j - 1);
                    }
                    //DamageLocation();
                    //DamageLocation d = getDamageRaw(i - 1, j - 1) + damage;
                    //setDamageRaw( i-1, j-1, d);
                }
            }
        }
    }

    void setEnemyDamageRadius(Point2D pos, float radius, DamageLocation damage) {
        Profiler profiler("DamageGridF");
        //printf("5ize:%d\n", sizeof(DamageLocation));
        enemyDamageNetModify->clear();
        int x = (int)((pos.x - radius) * damageNetPrecision);
        int y = (int)((pos.y - radius) * damageNetPrecision);
        int xmax = (int)((pos.x + radius) * damageNetPrecision);
        int ymax = (int)((pos.y + radius) * damageNetPrecision);
        //printf("%d - %d, %d - %d\n", x, xmax, y, ymax);
        imRef(enemyDamageNetModify, int(pos.x * damageNetPrecision), int(pos.y * damageNetPrecision)) = 1;
        for (int i = x; i <= xmax; i++) {
            for (int j = y; j <= ymax; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                float f = Distance2D(pos * damageNetPrecision, Point2D{ (float)i,(float)j });
                if (i > 1 && i < enemyDamageNetModify->width() && j > 1 && j < enemyDamageNetModify->height() && f < radius * damageNetPrecision) {
                    imRef(enemyDamageNetModify, i, j) = 1;
                    imRef(enemyDamageNetModify, i, j - 1) = 1;
                    imRef(enemyDamageNetModify, i - 1, j) = 1;
                    imRef(enemyDamageNetModify, i - 1, j - 1) = 1;
                }
            }
        }

        for (int i = x-1; i <= xmax+1; i++) {
            for (int j = y-1; j <= ymax+1; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                if (i > 1 && i < enemyDamageNetModify->width() && j > 1 && j < enemyDamageNetModify->height() && imRef(enemyDamageNetModify, i, j)) {
                    setDamageNetEnemyUnscaled(i - 1, j - 1, getDamageNetEnemyUnscaled(i - 1, j - 1) + damage);
                }
            }
        }
    }

    void fillDamageModify2(Point2D pos, float radius) {
        int x = (int)((pos.x - radius) * damageNetPrecision);
        int y = (int)((pos.y - radius) * damageNetPrecision);
        int xmax = (int)((pos.x + radius) * damageNetPrecision);
        int ymax = (int)((pos.y + radius) * damageNetPrecision);
        //printf("%d - %d, %d - %d\n", x, xmax, y, ymax);
        imRef(enemyDamageNetModify, int(pos.x * damageNetPrecision), int(pos.y * damageNetPrecision)) = 1;
        for (int i = x; i <= xmax; i++) {
            for (int j = y; j <= ymax; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                float f = Distance2D(pos * damageNetPrecision, Point2D{ (float)i,(float)j });
                if (i > 1 && i < enemyDamageNetModify->width() && j > 1 && j < enemyDamageNetModify->height() && f < radius * damageNetPrecision) {
                    imRef(enemyDamageNetModify, i, j) = 1;
                    imRef(enemyDamageNetModify, i, j - 1) = 1;
                    imRef(enemyDamageNetModify, i - 1, j) = 1;
                    imRef(enemyDamageNetModify, i - 1, j - 1) = 1;
                }
            }
        }
    }

    void fillDamageModify(Point2D pos, float radius) {
        if (radius < 1) {
            fillDamageModify2(pos, radius);
            return;
        }
        int center_x = (int)(pos.x * damageNetPrecision);
        int center_y = (int)(pos.y * damageNetPrecision);

        int xmin = std::max(int((pos.x - radius) * damageNetPrecision), 0);
        int ymin = std::max(int((pos.y - radius) * damageNetPrecision), 0);
        int xmax = std::min(int((pos.x + radius) * damageNetPrecision), enemyDamageNetModify->width());
        int ymax = std::min(int((pos.y + radius) * damageNetPrecision), enemyDamageNetModify->height());

        Point2D starting = pos + Point2D{ radius, 0 };
        int operating_x = int(starting.x * damageNetPrecision);
        int operating_y = int(starting.y * damageNetPrecision);
        if (operating_x >= 0 && operating_x < enemyDamageNetModify->width() && operating_y >= 0 && operating_y < enemyDamageNetModify->height()) {
            imRef(enemyDamageNetModify, operating_x, operating_y) = 1;
        }
        

        int dir_x = 0;
        int dir_y = 0;

        for (int p = 0; p < (((radius + 2) * damageNetPrecision) * 6); p++) {
            if (operating_x == center_x) {
                if (operating_y > center_y) {
                    dir_x = 1;
                    dir_y = -1;
                }
                else if (operating_y < center_y) {
                    dir_x = -1;
                    dir_y = 1;
                }
                else {
                    printf("CENTER X\n");
                }
            }
            else if (operating_y == center_y) {
                if (operating_x > center_x) {
                    if (dir_x != 0) {
                        dir_x = 2;
                        break;
                    }
                    dir_x = -1;
                    dir_y = -1;
                }
                else if (operating_x < center_x) {
                    dir_x = 1;
                    dir_y = 1;
                }
                else {
                    printf("CENTER Y\n");
                }
            }
            else {
                //printf("CENTER XY %d\n",p);
            }
            int min_x = 0;
            int min_y = 0;
            float dist = -1;
            for (int disp_x : {0, dir_x}) {
                for (int disp_y : {0, dir_y}) {
                    if (disp_x == 0 && disp_y == 0) {
                        continue;
                    }
                    Point2D testPoint{ (operating_x + disp_x + 0.5F) / damageNetPrecision, (operating_y + disp_y + 0.5F) / damageNetPrecision };
                    float distPoint = Distance2D(pos, testPoint);
                    if (distPoint < (radius + 0.5F / damageNetPrecision)) {
                        if (dist == -1 || dist < distPoint) {
                            dist = distPoint;
                            min_x = disp_x;
                            min_y = disp_y;
                        }
                    }
                }
            }
            operating_x += min_x;
            operating_y += min_y;
            if (operating_x >= 0 && operating_x < enemyDamageNetModify->width() && operating_y >= 0 && operating_y < enemyDamageNetModify->height()) {
                imRef(enemyDamageNetModify, operating_x, operating_y) = 1;
            }

        }

        for (int yi = ymin; yi < ymax; yi++) {
            for (int xi = center_x; xi < xmax; xi++) {
                if (imRef(enemyDamageNetModify, xi, yi)) {
                    break;
                }
                imRef(enemyDamageNetModify, xi, yi) = 1;
            }
            for (int xi = center_x - 1; xi > xmin; xi--) {
                if (imRef(enemyDamageNetModify, xi, yi)) {
                    break;
                }
                imRef(enemyDamageNetModify, xi, yi) = 1;
            }
        }
    }

    
    void setEnemyDamageRadius3(Point2D pos, float radius, DamageLocation damage) {
        //Profiler profiler("DamageGridF");
        enemyDamageNetModify->clear();
        
        int xmin = std::max(int((pos.x - radius) * damageNetPrecision), 0);
        int ymin = std::max(int((pos.y - radius) * damageNetPrecision), 0);
        int xmax = std::min(int((pos.x + radius) * damageNetPrecision), enemyDamageNetModify->width());
        int ymax = std::min(int((pos.y + radius) * damageNetPrecision), enemyDamageNetModify->height());

        fillDamageModify(pos, radius);

        for (int i = xmin; i <= xmax; i++) {
            for (int j = ymin; j <= ymax; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                if (imRef(enemyDamageNetModify, i, j)) {
                    setDamageNetEnemyUnscaled(i, j, getDamageNetEnemyUnscaled(i, j) + damage);
                }
            }
        }
    }

    DamageLocation getRadiusMaxDamage(Point2D pos, float radius) {
        //Profiler profiler("DamageGridF");
        enemyDamageNetModify->clear();

        int xmin = std::max(int((pos.x - radius) * damageNetPrecision), 0);
        int ymin = std::max(int((pos.y - radius) * damageNetPrecision), 0);
        int xmax = std::min(int((pos.x + radius) * damageNetPrecision), enemyDamageNetModify->width());
        int ymax = std::min(int((pos.y + radius) * damageNetPrecision), enemyDamageNetModify->height());

        fillDamageModify(pos, radius);

        DamageLocation d;

        for (int i = xmin; i <= xmax; i++) {
            for (int j = ymin; j <= ymax; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                if (imRef(enemyDamageNetModify, i, j)) {
                    //DamageLocation pointDmag = imRef(enemyDamageNet, i, j);
                    d.updateHighest(getDamageNetEnemyUnscaled(i, j));
                }
            }
        }
        return d;
    }

    DamageLocation getRadiusAvgDamage(Point2D pos, float radius) {
        //Profiler profiler("DamageGridF");
        enemyDamageNetModify->clear();

        int xmin = std::max(int((pos.x - radius) * damageNetPrecision), 0);
        int ymin = std::max(int((pos.y - radius) * damageNetPrecision), 0);
        int xmax = std::min(int((pos.x + radius) * damageNetPrecision), enemyDamageNetModify->width());
        int ymax = std::min(int((pos.y + radius) * damageNetPrecision), enemyDamageNetModify->height());

        fillDamageModify(pos, radius);

        DamageLocation d;
        int count = 0;
        for (int i = xmin; i <= xmax; i++) {
            for (int j = ymin; j <= ymax; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                if (imRef(enemyDamageNetModify, i, j)) {
                    //DamageLocation pointDmag = imRef(enemyDamageNet, i, j);
                    //d.updateHighest(imRef(enemyDamageNet, i, j));
                    d += getDamageNetEnemyUnscaled(i, j);
                    count++;
                }
            }
        }
        return (d/((float)count));
    }

    DamageLocation WeaponToDamageLocation(Weapon w) {
        DamageLocation d;
        Damage damage;
        for (DamageBonus bonus : w.damage_bonus) {
            if (bonus.attribute == Attribute::Light) {
                damage.light += (uint16_t)(bonus.bonus / w.speed * 100 * w.attacks);
            }
            else if (bonus.attribute == Attribute::Armored) {
                damage.armored += (uint16_t)(bonus.bonus / w.speed * 100 * w.attacks);
            }
            else if (bonus.attribute == Attribute::Biological) {
                damage.biological += (uint16_t)(bonus.bonus / w.speed * 100 * w.attacks);
            }
            else if (bonus.attribute == Attribute::Mechanical) {
                damage.mechanical += (uint16_t)(bonus.bonus / w.speed * 100 * w.attacks);
            }
            else if (bonus.attribute == Attribute::Massive) {
                damage.massive += (uint16_t)(bonus.bonus / w.speed * 100 * w.attacks);
            }
            else if (bonus.attribute == Attribute::Psionic) {
                damage.psionic += (uint16_t)(bonus.bonus / w.speed * 100 * w.attacks);
            }
            else {
                printf("ATTRIBUTE NOT PROGRAMMED %ud\n", bonus.attribute);
            }
        }

        if (w.type == Weapon::TargetType::Air || w.type == Weapon::TargetType::Any) {
            d.air.normal = (uint16_t)(w.damage_ / w.speed * 100 * w.attacks);
            d.air += damage;
        }
        if (w.type == Weapon::TargetType::Ground || w.type == Weapon::TargetType::Any) {
            d.ground.normal = (uint16_t)(w.damage_ / w.speed * 100 * w.attacks);
            d.ground += damage;
        }
        return d;
    }

    float getRelevantDamage(UnitWrapper* unitWrap, DamageLocation pointDamage, Agent* agent) {
        Composition comp = unitWrap->getComposition(agent);
        float damage = 0;
        if (comp == Composition::Ground || comp == Composition::Any) {
            damage += pointDamage.ground.normal;
            for (Attribute a : Aux::getStats(unitWrap->getType(agent), agent).attributes) {
                switch (a) {
                    case(Attribute::Light): {
                        damage += pointDamage.ground.light;
                    } break;
                    case(Attribute::Armored): {
                        damage += pointDamage.ground.armored;
                    } break;
                    case(Attribute::Biological): {
                        damage += pointDamage.ground.biological;
                    } break;
                    case(Attribute::Mechanical): {
                        damage += pointDamage.ground.mechanical;
                    } break;
                    case(Attribute::Massive): {
                        damage += pointDamage.ground.massive;
                    } break;
                    case(Attribute::Psionic): {
                        damage += pointDamage.ground.psionic;
                    } break;
                }
            }

        }
        if (comp == Composition::Air || comp == Composition::Any) {
            damage += pointDamage.air.normal;
            for (Attribute a : Aux::getStats(unitWrap->getType(agent), agent).attributes) {
                switch (a) {
                    case(Attribute::Light): {
                        damage += pointDamage.air.light;
                    } break;
                    case(Attribute::Armored): {
                        damage += pointDamage.air.armored;
                    } break;
                    case(Attribute::Biological): {
                        damage += pointDamage.air.biological;
                    } break;
                    case(Attribute::Mechanical): {
                        damage += pointDamage.air.mechanical;
                    } break;
                    case(Attribute::Massive): {
                        damage += pointDamage.air.massive;
                    } break;
                    case(Attribute::Psionic): {
                        damage += pointDamage.air.psionic;
                    } break;
                }
            }

        }
        return damage;
    }

    float getRelevantDamage(UnitWrapper* target, Weapon w, Agent* agent) {
        return getRelevantDamage(target, WeaponToDamageLocation(w), agent);
    }
    
    float getPointDamage(int i, int j, UnitWrapper* unitWrap, Agent* agent) {
        return getRelevantDamage(unitWrap, getDamageNetEnemyUnscaled(i, j), agent);
    }

    Point2D findMinimumDamage(UnitWrapper* unitWrap, float radius, Agent* agent) {
        //printf("5ize:%d\n", sizeof(DamageLocation));
        enemyDamageNetTemp->clear();
        Point2D pos = unitWrap->pos(agent);
        int x = (int)((pos.x - radius) * damageNetPrecision);
        int y = (int)((pos.y - radius) * damageNetPrecision);
        int xmax = (int)((pos.x + radius) * damageNetPrecision);
        int ymax = (int)((pos.y + radius) * damageNetPrecision);
        //printf("%d - %d, %d - %d\n", x, xmax, y, ymax);
        imRef(enemyDamageNetModify, int(pos.x * damageNetPrecision), int(pos.y * damageNetPrecision)) = 1;
        for (int i = x; i <= xmax; i++) {
            for (int j = y; j <= ymax; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                float f = Distance2D(pos * damageNetPrecision, Point2D{ (float)i,(float)j });
                if (i > 1 && i < enemyDamageNetModify->width() && j > 1 && j < enemyDamageNetModify->height() && f < radius * damageNetPrecision) {
                    //imRef(enemyDamageNetModify, i, j) = 1;
                    //imRef(enemyDamageNetModify, i, j - 1) = 1;
                    //imRef(enemyDamageNetModify, i - 1, j) = 1;
                    //imRef(enemyDamageNetModify, i - 1, j - 1) = 1;
                    if (imRef(enemyDamageNetTemp, i, j) == 0) {
                        imRef(enemyDamageNetTemp, i, j) = getPointDamage(i, j, unitWrap, agent);
                    }
                    if (imRef(enemyDamageNetTemp, i, j - 1) == 0) {
                        imRef(enemyDamageNetTemp, i, j - 1) = getPointDamage(i, j - 1, unitWrap, agent);
                    }
                    if (imRef(enemyDamageNetTemp, i - 1, j) == 0) {
                        imRef(enemyDamageNetTemp, i - 1, j) = getPointDamage(i - 1, j, unitWrap, agent);
                    }
                    if (imRef(enemyDamageNetTemp, i - 1, j - 1) == 0) {
                        imRef(enemyDamageNetTemp, i - 1, j - 1) = getPointDamage(i - 1, j - 1, unitWrap, agent);
                    }
                }
            }
        }
        int mini = 0;
        int minj = 0;
        float mindamage = -1;
        for (int i = x - 1; i <= xmax + 1; i++) {
            for (int j = y - 1; j <= ymax + 1; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                if (i > 1 && i < enemyDamageNetModify->width() && j > 1 && j < enemyDamageNetModify->height() && imRef(enemyDamageNetTemp, i - 1, j - 1) != 0) {
                    float damage = imRef(enemyDamageNetTemp, i, j);
                    /*printf("%.1f @ %.1f,%.1f\n", damage, i * blockSize, j * blockSize);*/
                    if (mini == 0 || minj == 0 || mindamage == -1 || mindamage > damage) {
                        mini = i;
                        minj = j;
                        mindamage = damage;
                    }
                }
            }
        }
        printf("%.1f @ %.1f,%.1f\n", mindamage, mini * blockSize, minj * blockSize);
        //DebugLine(agent,Point3D{ mini * blockSize, minj * blockSize, 0.0F }, Point3D{ mini * blockSize, minj * blockSize, 13.0F });
        return Point2D{ mini * blockSize, minj * blockSize };
    }

    Point2D weightedVector34745(UnitWrapper* unitWrap, float radius, Agent* agent) {
        //printf("5ize:%d\n", sizeof(DamageLocation));
        enemyDamageNetTemp->clear();
        Point2D pos = unitWrap->pos(agent);
        int x = (int)((pos.x - radius) * damageNetPrecision);
        int y = (int)((pos.y - radius) * damageNetPrecision);
        int xmax = (int)((pos.x + radius) * damageNetPrecision);
        int ymax = (int)((pos.y + radius) * damageNetPrecision);
        //printf("%d - %d, %d - %d\n", x, xmax, y, ymax);
        imRef(enemyDamageNetModify, int(pos.x * damageNetPrecision), int(pos.y * damageNetPrecision)) = 1;
        for (int i = x; i <= xmax; i++) {
            for (int j = y; j <= ymax; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                float f = Distance2D(pos * damageNetPrecision, Point2D{ (float)i,(float)j });
                if (i > 1 && i < enemyDamageNetModify->width() && j > 1 && j < enemyDamageNetModify->height() && f < radius * damageNetPrecision) {
                    //imRef(enemyDamageNetModify, i, j) = 1;
                    //imRef(enemyDamageNetModify, i, j - 1) = 1;
                    //imRef(enemyDamageNetModify, i - 1, j) = 1;
                    //imRef(enemyDamageNetModify, i - 1, j - 1) = 1;
                    if (imRef(enemyDamageNetTemp, i, j) == 0) {
                        imRef(enemyDamageNetTemp, i, j) = getPointDamage(i, j, unitWrap, agent);
                    }
                    if (imRef(enemyDamageNetTemp, i, j - 1) == 0) {
                        imRef(enemyDamageNetTemp, i, j - 1) = getPointDamage(i, j - 1, unitWrap, agent);
                    }
                    if (imRef(enemyDamageNetTemp, i - 1, j) == 0) {
                        imRef(enemyDamageNetTemp, i - 1, j) = getPointDamage(i - 1, j, unitWrap, agent);
                    }
                    if (imRef(enemyDamageNetTemp, i - 1, j - 1) == 0) {
                        imRef(enemyDamageNetTemp, i - 1, j - 1) = getPointDamage(i - 1, j - 1, unitWrap, agent);
                    }
                }
            }
        }
        Point2D vector;
        //int count = 0;
        for (int i = x - 1; i <= xmax + 1; i++) {
            for (int j = y - 1; j <= ymax + 1; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                if (i > 1 && i < enemyDamageNetModify->width() && j > 1 && j < enemyDamageNetModify->height() && imRef(enemyDamageNetModify, i, j) && agent->Observation()->IsPathable(Point2D{(i + 0.5F)*blockSize,(j + 0.5F) * blockSize })) {
                    float damage = imRef(enemyDamageNetTemp, i, j);
                    Point2D dir = normalize(Point2D{ (float)(int)(pos.x * damageNetPrecision), (float)(int)(pos.y * damageNetPrecision) });
                    //printf("%.1f @ %.1f,%.1f\n", damage, i * blockSize, j * blockSize);
                    vector += (dir * damage);
                    //count++;
                }
            }
        }
        //DebugLine(agent,Point3D{ mini * blockSize, minj * blockSize, 0.0F }, Point3D{ mini * blockSize, minj * blockSize, 13.0F });
        return normalize(vector);
    }
    
    bool checkExist(UnitTypeID id) {
        return units.find(id) != units.end();
    }

    UnitWrappers get(UnitTypeID type) {
        if (checkExist(type)) {
            return units[type];
        }
        return UnitWrappers();
    }

    UnitWrappers getMulti(vector<UnitTypeID> types) {
        int total = 0;
        for (UnitTypeID type : types) {
            if (checkExist(type)) {
                total += (int)(units[type].size());
            }
        }
        UnitWrappers wraps;
        wraps.reserve(total);
        for (UnitTypeID type : types) {
            if (checkExist(type)) {
                for (UnitWrapper* wrap : units[type]) {
                    wraps.push_back(wrap);
                }
            }
        }
        return wraps;
    }

    UnitWrapper* find(UnitTypeID type, Tag tag) {
        UnitWrappers v = get(type);
        for (int i = 0; i < v.size(); i++) {
            if (v[i]->self == tag) {
                return v[i];
            }
        }
        return nullptr;
    }

    bool checkExistNeutral(UnitTypeID id) {
        return neutrals.find(id) != neutrals.end();
    }

    UnitWrappers getNeutral(UnitTypeID type) {
        if (checkExistNeutral(type)) {
            return neutrals[type];
        }
        return UnitWrappers();
    }

    UnitWrappers getNeutralMulti(vector<UnitTypeID> types) {
        int total = 0;
        for (UnitTypeID type : types) {
            if (checkExistNeutral(type)) {
                total += (int)(neutrals[type].size());
            }
        }
        UnitWrappers wraps;
        wraps.reserve(total);
        for (UnitTypeID type : types) {
            if (checkExistNeutral(type)) {
                for (UnitWrapper* wrap : neutrals[type]) {
                    wraps.push_back(wrap);
                }
            }
        }
        return wraps;
    }

    UnitWrappers getVespene() {
        //UnitWrappers v1 = getNeutral(UNIT_TYPEID::NEUTRAL_VESPENEGEYSER);
        //UnitWrappers v2 = getNeutral(UNIT_TYPEID::NEUTRAL_PROTOSSVESPENEGEYSER);
        //UnitWrappers v3 = getNeutral(UNIT_TYPEID::NEUTRAL_PURIFIERVESPENEGEYSER);
        //UnitWrappers v4 = getNeutral(UNIT_TYPEID::NEUTRAL_RICHVESPENEGEYSER);
        //UnitWrappers v5 = getNeutral(UNIT_TYPEID::NEUTRAL_SHAKURASVESPENEGEYSER);
        //UnitWrappers v6 = getNeutral(UNIT_TYPEID::NEUTRAL_SPACEPLATFORMGEYSER);
        //v1.insert(v1.end(), v2.begin(), v2.end());
        //v1.insert(v1.end(), v3.begin(), v3.end());
        //v1.insert(v1.end(), v4.begin(), v4.end());
        //v1.insert(v1.end(), v5.begin(), v5.end());
        //v1.insert(v1.end(), v6.begin(), v6.end());
        //return v1;
        vector<UnitTypeID> vespenes = {
            UNIT_TYPEID::NEUTRAL_VESPENEGEYSER,
            UNIT_TYPEID::NEUTRAL_PROTOSSVESPENEGEYSER,
            UNIT_TYPEID::NEUTRAL_PURIFIERVESPENEGEYSER,
            UNIT_TYPEID::NEUTRAL_RICHVESPENEGEYSER,
            UNIT_TYPEID::NEUTRAL_SHAKURASVESPENEGEYSER,
            UNIT_TYPEID::NEUTRAL_SPACEPLATFORMGEYSER };
        return getNeutralMulti(vespenes);
    }

    UnitWrappers getMinerals() {
        //UnitWrappers v1 = getNeutral(UNIT_TYPEID::NEUTRAL_MINERALFIELD);
        //UnitWrappers v2 = getNeutral(UNIT_TYPEID::NEUTRAL_LABMINERALFIELD);
        //UnitWrappers v3 = getNeutral(UNIT_TYPEID::NEUTRAL_MINERALFIELD750);
        //UnitWrappers v4 = getNeutral(UNIT_TYPEID::NEUTRAL_LABMINERALFIELD750);
        //UnitWrappers v5 = getNeutral(UNIT_TYPEID::NEUTRAL_MINERALFIELD450);
        //v1.insert(v1.end(), v2.begin(), v2.end());
        //v1.insert(v1.end(), v3.begin(), v3.end());
        //v1.insert(v1.end(), v4.begin(), v4.end());
        //v1.insert(v1.end(), v5.begin(), v5.end());
        //return v1;
        vector<UnitTypeID> minerals = {
            UNIT_TYPEID::NEUTRAL_MINERALFIELD,
            UNIT_TYPEID::NEUTRAL_LABMINERALFIELD,
            UNIT_TYPEID::NEUTRAL_MINERALFIELD750,
            UNIT_TYPEID::NEUTRAL_LABMINERALFIELD750,
            UNIT_TYPEID::NEUTRAL_MINERALFIELD450};
        return getNeutralMulti(minerals);
    }

    UnitWrapper *findNeutral(UnitTypeID type, Tag tag) {
        UnitWrappers v = getNeutral(type);
        for (int i = 0; i < v.size(); i++) {
            if (v[i]->self == tag) {
                return v[i];
            }
        }
        return nullptr;
    }

    bool checkExistEnemy(UnitTypeID id) {
        return enemies.find(id) != enemies.end();
    }

    UnitWrappers getEnemy(UnitTypeID type) {
        if (checkExistEnemy(type)) {
            return enemies[type];
        }
        return UnitWrappers();
    }

    UnitWrappers getEnemyMulti(vector<UnitTypeID> types) {
        int total = 0;
        for (UnitTypeID type : types) {
            if (checkExistEnemy(type)) {
                total += (int)(enemies[type].size());
            }
        }
        UnitWrappers wraps;
        wraps.reserve(total);
        for (UnitTypeID type : types) {
            if (checkExistEnemy(type)) {
                for (UnitWrapper* wrap : enemies[type]) {
                    wraps.push_back(wrap);
                }
            }
        }
        return wraps;
    }

    UnitWrapper *findEnemy(UnitTypeID type, Tag tag) {
        UnitWrappers v = getEnemy(type);
        for (int i = 0; i < v.size(); i++) {
            if (v[i]->self == tag) {
                return v[i];
            }
        }
        return nullptr;
    }
}  // namespace UnitManager


//UnitWrapper::UnitWrapper(Tag self_) : self(self_), type(UNIT_TYPEID::INVALID), lastPos{0, 0} {
//    printf("WHY ARE YOU USING THE DEFAULT UNITWRAPPER CONSTRUCTOR\n");
//}

//UnitWrapper::UnitWrapper(Tag self_, UnitTypeID type_)
//    : self(self_), type(type_), radius(0){
//    if (!UnitManager::checkExist(type)) {
//            UnitManager::units[type] = UnitWrappers();
//    }
//    UnitManager::units[type].push_back(this);
//}

map<UnitTypeID, UnitTypeID> superTypes = { 
    {UNIT_TYPEID::PROTOSS_ASSIMILATORRICH, UNIT_TYPEID::PROTOSS_ASSIMILATOR},
    {UNIT_TYPEID::PROTOSS_WARPPRISMPHASING, UNIT_TYPEID::PROTOSS_WARPPRISM},
    {UNIT_TYPEID::PROTOSS_DISRUPTORPHASED, UNIT_TYPEID::PROTOSS_DISRUPTOR},
    {UNIT_TYPEID::PROTOSS_OBSERVERSIEGEMODE, UNIT_TYPEID::PROTOSS_OBSERVER},

    {UNIT_TYPEID::TERRAN_BARRACKSREACTOR, UNIT_TYPEID::TERRAN_REACTOR},
    {UNIT_TYPEID::TERRAN_BARRACKSTECHLAB, UNIT_TYPEID::TERRAN_TECHLAB},
    {UNIT_TYPEID::TERRAN_FACTORYREACTOR, UNIT_TYPEID::TERRAN_REACTOR},
    {UNIT_TYPEID::TERRAN_FACTORYTECHLAB, UNIT_TYPEID::TERRAN_TECHLAB},
    {UNIT_TYPEID::TERRAN_STARPORTREACTOR, UNIT_TYPEID::TERRAN_REACTOR},
    {UNIT_TYPEID::TERRAN_STARPORTTECHLAB, UNIT_TYPEID::TERRAN_TECHLAB},
    {UNIT_TYPEID::TERRAN_REFINERYRICH, UNIT_TYPEID::TERRAN_REFINERY},
    {UNIT_TYPEID::TERRAN_BARRACKSFLYING, UNIT_TYPEID::TERRAN_BARRACKS},
    {UNIT_TYPEID::TERRAN_COMMANDCENTERFLYING, UNIT_TYPEID::TERRAN_COMMANDCENTER},
    {UNIT_TYPEID::TERRAN_ORBITALCOMMAND, UNIT_TYPEID::TERRAN_COMMANDCENTER},
    {UNIT_TYPEID::TERRAN_ORBITALCOMMANDFLYING, UNIT_TYPEID::TERRAN_COMMANDCENTER},
    {UNIT_TYPEID::TERRAN_PLANETARYFORTRESS, UNIT_TYPEID::TERRAN_COMMANDCENTER},
    {UNIT_TYPEID::TERRAN_FACTORYFLYING, UNIT_TYPEID::TERRAN_FACTORY},
    {UNIT_TYPEID::TERRAN_HELLIONTANK, UNIT_TYPEID::TERRAN_HELLION},
    {UNIT_TYPEID::TERRAN_LIBERATORAG, UNIT_TYPEID::TERRAN_LIBERATOR},
    {UNIT_TYPEID::TERRAN_VIKINGASSAULT, UNIT_TYPEID::TERRAN_VIKINGFIGHTER},
    {UNIT_TYPEID::TERRAN_HELLIONTANK, UNIT_TYPEID::TERRAN_HELLION},
    {UNIT_TYPEID::TERRAN_SIEGETANKSIEGED, UNIT_TYPEID::TERRAN_SIEGETANK},
    {UNIT_TYPEID::TERRAN_STARPORTFLYING, UNIT_TYPEID::TERRAN_STARPORT},
    {UNIT_TYPEID::TERRAN_SUPPLYDEPOTLOWERED, UNIT_TYPEID::TERRAN_SUPPLYDEPOT},
    {UNIT_TYPEID::TERRAN_THORAP, UNIT_TYPEID::TERRAN_THOR},
    {UNIT_TYPEID::TERRAN_WIDOWMINEBURROWED, UNIT_TYPEID::TERRAN_WIDOWMINE},

    {UNIT_TYPEID::ZERG_EXTRACTORRICH, UNIT_TYPEID::ZERG_EXTRACTOR},
    {UNIT_TYPEID::ZERG_BANELINGBURROWED, UNIT_TYPEID::ZERG_BANELING},
    {UNIT_TYPEID::ZERG_BANELINGCOCOON, UNIT_TYPEID::ZERG_BANELING},
    {UNIT_TYPEID::ZERG_CHANGELINGMARINE, UNIT_TYPEID::ZERG_CHANGELING},
    {UNIT_TYPEID::ZERG_CHANGELINGMARINESHIELD, UNIT_TYPEID::ZERG_CHANGELING},
    {UNIT_TYPEID::ZERG_CHANGELINGZEALOT, UNIT_TYPEID::ZERG_CHANGELING},
    {UNIT_TYPEID::ZERG_CHANGELINGZERGLING, UNIT_TYPEID::ZERG_CHANGELING},
    {UNIT_TYPEID::ZERG_CHANGELINGZERGLINGWINGS, UNIT_TYPEID::ZERG_CHANGELING},
    {UNIT_TYPEID::ZERG_CREEPTUMORBURROWED, UNIT_TYPEID::ZERG_CREEPTUMOR},
    {UNIT_TYPEID::ZERG_CREEPTUMORQUEEN, UNIT_TYPEID::ZERG_CREEPTUMOR},
    {UNIT_TYPEID::ZERG_DRONEBURROWED, UNIT_TYPEID::ZERG_DRONE},
    {UNIT_TYPEID::ZERG_HYDRALISKBURROWED, UNIT_TYPEID::ZERG_HYDRALISK},
    {UNIT_TYPEID::ZERG_INFESTORBURROWED, UNIT_TYPEID::ZERG_INFESTOR},
    {UNIT_TYPEID::ZERG_LOCUSTMPFLYING, UNIT_TYPEID::ZERG_LOCUSTMP},
    {UNIT_TYPEID::ZERG_LURKERMPEGG, UNIT_TYPEID::ZERG_LURKERMP},
    {UNIT_TYPEID::ZERG_LURKERMPBURROWED, UNIT_TYPEID::ZERG_LURKERMP},
    {UNIT_TYPEID::ZERG_OVERLORDCOCOON, UNIT_TYPEID::ZERG_OVERLORD},
    {UNIT_TYPEID::ZERG_OVERSEERSIEGEMODE, UNIT_TYPEID::ZERG_OVERSEER},
    {UNIT_TYPEID::ZERG_QUEENBURROWED, UNIT_TYPEID::ZERG_QUEEN},
    {UNIT_TYPEID::ZERG_RAVAGERCOCOON, UNIT_TYPEID::ZERG_RAVAGER},
    {UNIT_TYPEID::ZERG_ROACHBURROWED, UNIT_TYPEID::ZERG_ROACH},
    {UNIT_TYPEID::ZERG_SPINECRAWLERUPROOTED, UNIT_TYPEID::ZERG_SPINECRAWLER},
    {UNIT_TYPEID::ZERG_SPORECRAWLERUPROOTED, UNIT_TYPEID::ZERG_SPORECRAWLER},
    {UNIT_TYPEID::ZERG_SWARMHOSTBURROWEDMP, UNIT_TYPEID::ZERG_SWARMHOSTMP},
    {UNIT_TYPEID::ZERG_ULTRALISKBURROWED, UNIT_TYPEID::ZERG_ULTRALISK},
    {UNIT_TYPEID::ZERG_TRANSPORTOVERLORDCOCOON, UNIT_TYPEID::ZERG_OVERLORDTRANSPORT},
    {UNIT_TYPEID::ZERG_ZERGLINGBURROWED, UNIT_TYPEID::ZERG_ZERGLING},
    {UNIT_TYPEID::ZERG_LAIR, UNIT_TYPEID::ZERG_HATCHERY},
    {UNIT_TYPEID::ZERG_HIVE, UNIT_TYPEID::ZERG_HATCHERY},
};

UnitTypeID getSuperType(UnitTypeID in) {
    if (superTypes.find(in) == superTypes.end()) {
        return in;
    }
    else {
        return superTypes.at(in);
    }
}

UnitWrapper::UnitWrapper(const Unit *unit) : self(unit->tag), actualType(unit->unit_type), lastPos{0, 0, 0}, radius(unit->radius), team(unit->alliance), isBuilding(unit->is_building), health(0), healthMax(0), shields(0), shieldsMax(0), hallucination(unit->is_hallucination) {
    type = getSuperType(actualType);

    if (unit->alliance == Unit::Alliance::Self) {
        if (!UnitManager::checkExist(type)) {
            UnitManager::units[type] = UnitWrappers();
        }
        UnitManager::units[type].push_back(this);
    }else if (unit->alliance == Unit::Alliance::Neutral) {
        if (!UnitManager::checkExistNeutral(type)) {
            UnitManager::neutrals[type] = UnitWrappers();
        }
        UnitManager::neutrals[type].push_back(this);
    }else if (unit->alliance == Unit::Alliance::Enemy) {
        if (!UnitManager::checkExistEnemy(type)) {
            UnitManager::enemies[type] = UnitWrappers();
        }
        //UnitManager::enemies[type].push_back(this);
        bool found = false;
        for (UnitWrapper* wrap : UnitManager::enemies[type]) {
            if (unit->tag == wrap->self) {
                found = true;
                //printf("DuplicateEnemy O:%s N:%s\n", UnitTypeToName(wrap->type), UnitTypeToName(unit->unit_type));
                break;
            }
        }
        if (found) {
            //printf("DuplicateEnemy\n", UnitTypeToName());
        } else {
            UnitManager::enemies[type].push_back(this);
        }
    }
    ignoreFrames = 0;
    abilities = AvailableAbilities();
}

Point2D UnitWrapper::pos(Agent *agent) {
    const Unit *unit = agent->Observation()->GetUnit(self);
    if (unit != nullptr) {
        lastPos = unit->pos;
    } else if (agent->Observation()->GetVisibility(lastPos) == Visibility::Visible) {
        lastPos = {0, 0, 0};
    }
    return lastPos;
}

Point3D UnitWrapper::pos3D(Agent *agent) {
    const Unit *unit = agent->Observation()->GetUnit(self);
    if (unit != nullptr) {
        lastPos = unit->pos;
    } else if (agent->Observation()->GetVisibility(lastPos) == Visibility::Visible) {
        lastPos = {0, 0, 0};
    }
    return lastPos;
}

inline bool UnitWrapper::exists(Agent *agent) {
    return agent->Observation()->GetUnit(self) == nullptr;
}

inline const Unit* UnitWrapper::get(Agent *agent) {
    const Unit* unit = agent->Observation()->GetUnit(self);
    if (unit != nullptr && unit->is_hallucination) {
        hallucination = true;
    }
    return unit;
}

bool UnitWrapper::equals(UnitWrapper *wrapper) {
    return (wrapper->self == self) && (wrapper->type == type);
}

//We only care about nexus, probe, gateway, ?zealot?, forge, cybercore, sentry, stalker, adept, shield battery, twilight council, stargate, pheonix, oracle, void ray, robotics facility, observer, warp prism, templar, high templar, archon, dark shrine, dark templar, fleet beacon, ?carrier, mothership, robotics bay, disruptor
void UnitWrapper::loadAbilities(Agent *agent) {
    Units all;
    vector<UnitTypeID> types = {
        UNIT_TYPEID::PROTOSS_NEXUS,
        UNIT_TYPEID::PROTOSS_PROBE,
        UNIT_TYPEID::PROTOSS_GATEWAY,
        UNIT_TYPEID::PROTOSS_ZEALOT,
        UNIT_TYPEID::PROTOSS_FORGE,
        UNIT_TYPEID::PROTOSS_CYBERNETICSCORE,
        UNIT_TYPEID::PROTOSS_SENTRY,
        UNIT_TYPEID::PROTOSS_STALKER,
        UNIT_TYPEID::PROTOSS_ADEPT,
        UNIT_TYPEID::PROTOSS_SHIELDBATTERY,
        UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL,
        UNIT_TYPEID::PROTOSS_STARGATE,
        UNIT_TYPEID::PROTOSS_PHOENIX,
        UNIT_TYPEID::PROTOSS_ORACLE,
        UNIT_TYPEID::PROTOSS_VOIDRAY,
        UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY,
        UNIT_TYPEID::PROTOSS_OBSERVER,
        UNIT_TYPEID::PROTOSS_WARPPRISM,
        UNIT_TYPEID::PROTOSS_TEMPLARARCHIVE,
        UNIT_TYPEID::PROTOSS_HIGHTEMPLAR,
        UNIT_TYPEID::PROTOSS_ARCHON,
        UNIT_TYPEID::PROTOSS_DARKSHRINE,
        UNIT_TYPEID::PROTOSS_DARKTEMPLAR,
        UNIT_TYPEID::PROTOSS_FLEETBEACON,
        UNIT_TYPEID::PROTOSS_CARRIER,
        UNIT_TYPEID::PROTOSS_MOTHERSHIP,
        UNIT_TYPEID::PROTOSS_ROBOTICSBAY,
        UNIT_TYPEID::PROTOSS_DISRUPTOR
    };
    UnitWrappers unitsWithAbilities = UnitManager::getMulti(types);
    all.reserve(unitsWithAbilities.size());
    for (UnitWrapper* wrapper : unitsWithAbilities) {
        const Unit* unit = wrapper->get(agent);
        if (unit != nullptr) {
            all.push_back(unit);
        }
    }
    vector<AvailableAbilities> allAb = agent->Query()->GetAbilitiesForUnits(all);
    for (int i = 0; i < allAb.size(); i ++) {
        if (allAb[i].unit_tag != NullTag) {
            UnitWrapper* u = UnitManager::find(allAb[i].unit_type_id, allAb[i].unit_tag);
            if (u == nullptr) {
                continue;
            }
            /*u->abilities = allAb[i];*/
            u->abilities.unit_tag = allAb[i].unit_tag;
            u->abilities.unit_type_id = allAb[i].unit_type_id;
            for (int a = 0; a < allAb[i].abilities.size(); a++) {
                u->abilities.abilities.clear();
                u->abilities.abilities.push_back(allAb[i].abilities[a]);
            }
        }
    }
}

//void UnitWrapper::loadAbilitiesEnemy(Agent* agent) {
//    Units all = agent->Observation()->GetUnits(Unit::Alliance::Enemy);
//    vector<AvailableAbilities> allAb = agent->Query()->GetAbilitiesForUnits(all);
//    for (AvailableAbilities abil : allAb) {
//        if (abil.unit_tag != NullTag) {
//            UnitWrapper* u = UnitManager::findEnemy(abil.unit_type_id, abil.unit_tag);
//            if (u == nullptr) {
//                continue;
//            }
//            u->abilities = abil;
//            if (abil.abilities.size() != 0) {
//                printf("ABIL:%d %s\n", abil.abilities.size(), UnitTypeToName(abil.unit_tag));
//                for (auto a : abil.abilities) {
//                    printf("%s\n", AbilityTypeToName(a.ability_id));
//                }
//            }
//        }
//
//    }
//}

UnitWrapper::~UnitWrapper() {
    if (team == Unit::Alliance::Self) {
        for (auto it = UnitManager::units[type].begin(); it != UnitManager::units[type].end(); it++) {
            if ((*it)->self == self) {
                UnitManager::units[type].erase(it);
                break;
            }
        }
    } else if (team == Unit::Alliance::Neutral) {
        for (auto it = UnitManager::neutrals[type].begin(); it != UnitManager::neutrals[type].end(); it++) {
            if ((*it)->self == self) {
                UnitManager::neutrals[type].erase(it);
                break;
            }
        }
    } else if (team == Unit::Alliance::Enemy) {
        for (auto it = UnitManager::enemies[type].begin(); it != UnitManager::enemies[type].end(); it++) {
            if ((*it)->self == self) {
                UnitManager::enemies[type].erase(it);
                break;
            }
        }
    } else {
        printf("WHAT THE FUCK TEAM IS THIS UNIT ON\n");
    }

}

bool UnitWrapper::execute(Agent *agent) {
    return false;
}