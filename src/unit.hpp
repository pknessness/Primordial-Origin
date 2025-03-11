#pragma once
#include <sc2api/sc2_api.h>
#include <map>
#include "map2dFloat.hpp"
#include "unitpriority.hpp"

using namespace sc2;
using namespace std;

class UnitWrapper {
private:
    Composition c;
    
public:
    float health;
    float healthMax;
    float shields;
    float shieldsMax;

    Tag self;
    UnitTypeID type;
    Point3D lastPos;
    float radius;
    bool isBuilding;
    Unit::Alliance team;
    AvailableAbilities abilities;
    int8_t ignoreFrames;

    //UnitWrapper(Tag self_);

    //UnitWrapper(Tag self_, UnitTypeID id);

    UnitWrapper(const Unit* unit);

    bool equals(UnitWrapper *wrapper);

    inline bool exists(Agent *agent);

    inline const Unit *get(Agent *agent);

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

    virtual bool executeDamaged(Agent *agent, float health, float shields) {
        return false;
    }

    static void loadAbilities(Agent *agent);

    static void loadAbilitiesEnemy(Agent* agent);

    bool checkAbility(AbilityID ability) {
        for (int i = 0; i < abilities.abilities.size(); i++) {
            if (ability == abilities.abilities.at(i).ability_id) {
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
using damageval = uint16_t;

struct Damage {
    damageval normal;
    damageval armored;
    damageval light;
    damageval biological;
    damageval mechanical;
    damageval massive;
    damageval psionic;

    Damage() {
        normal = 0;
        armored = 0;
        light = 0;
        biological = 0;
        mechanical = 0;
        massive = 0;
        psionic = 0;
    }

    Damage(damageval normal,
    damageval armored,
    damageval light,
    damageval biological,
    damageval mechanical,
    damageval massive,
    damageval psionic) : normal(normal), armored(armored), light(light), biological(biological), mechanical(mechanical), massive(massive), psionic(psionic){
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
            (damageval)(normal + u.normal),
            (damageval)(armored + u.armored),
            (damageval)(light + u.light),
            (damageval)(biological + u.biological),
            (damageval)(mechanical + u.mechanical),
            (damageval)(massive + u.massive), 
            (damageval)(psionic + u.psionic) };
    }

    Damage operator/(const float& u) {
        return Damage{
            (damageval)(normal / u),
            (damageval)(armored / u),
            (damageval)(light / u),
            (damageval)(biological / u),
            (damageval)(mechanical / u),
            (damageval)(massive / u),
            (damageval)(psionic / u) };
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
    
    void setEnemyDamageRadius2(Point2D pos, float radius, DamageLocation damage, Agent* agent) {
        int x = (pos.x - radius) * damageNetPrecision;
        int y = (pos.y - radius) * damageNetPrecision;
        int xmax = (pos.x + radius) * damageNetPrecision + 1;
        int ymax = (pos.y + radius) * damageNetPrecision + 1;
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

    void setEnemyDamageRadius(Point2D pos, float radius, DamageLocation damage, Agent* agent) {
        Profiler profiler("DamageGridF");
        //printf("5ize:%d\n", sizeof(DamageLocation));
        enemyDamageNetModify->clear();
        int x = (pos.x - radius) * damageNetPrecision;
        int y = (pos.y - radius) * damageNetPrecision;
        int xmax = (pos.x + radius) * damageNetPrecision;
        int ymax = (pos.y + radius) * damageNetPrecision;
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

    void fillDamageModify2(Point2D pos, float radius, Agent* agent) {
        int x = (pos.x - radius) * damageNetPrecision;
        int y = (pos.y - radius) * damageNetPrecision;
        int xmax = (pos.x + radius) * damageNetPrecision;
        int ymax = (pos.y + radius) * damageNetPrecision;
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

    void fillDamageModify(Point2D pos, float radius, Agent* agent) {
        if (radius < 1) {
            fillDamageModify2(pos, radius, agent);
            return;
        }
        int center_x = pos.x * damageNetPrecision;
        int center_y = pos.y * damageNetPrecision;

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

    
    void setEnemyDamageRadius3(Point2D pos, float radius, DamageLocation damage, Agent* agent) {
        //Profiler profiler("DamageGridF");
        enemyDamageNetModify->clear();
        
        int xmin = std::max(int((pos.x - radius) * damageNetPrecision), 0);
        int ymin = std::max(int((pos.y - radius) * damageNetPrecision), 0);
        int xmax = std::min(int((pos.x + radius) * damageNetPrecision), enemyDamageNetModify->width());
        int ymax = std::min(int((pos.y + radius) * damageNetPrecision), enemyDamageNetModify->height());

        fillDamageModify(pos, radius, agent);

        for (int i = xmin; i <= xmax; i++) {
            for (int j = ymin; j <= ymax; j++) {
                //DebugLine(agent,Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 0.0F }, Point3D{ (float)(i) / damageNetPrecision, (float)(j) / damageNetPrecision, 13.0F });
                if (imRef(enemyDamageNetModify, i, j)) {
                    setDamageNetEnemyUnscaled(i, j, getDamageNetEnemyUnscaled(i, j) + damage);
                }
            }
        }
    }

    DamageLocation getRadiusMaxDamage(Point2D pos, float radius, Agent* agent) {
        //Profiler profiler("DamageGridF");
        enemyDamageNetModify->clear();

        int xmin = std::max(int((pos.x - radius) * damageNetPrecision), 0);
        int ymin = std::max(int((pos.y - radius) * damageNetPrecision), 0);
        int xmax = std::min(int((pos.x + radius) * damageNetPrecision), enemyDamageNetModify->width());
        int ymax = std::min(int((pos.y + radius) * damageNetPrecision), enemyDamageNetModify->height());

        fillDamageModify(pos, radius, agent);

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

    DamageLocation getRadiusAvgDamage(Point2D pos, float radius, Agent* agent) {
        //Profiler profiler("DamageGridF");
        enemyDamageNetModify->clear();

        int xmin = std::max(int((pos.x - radius) * damageNetPrecision), 0);
        int ymin = std::max(int((pos.y - radius) * damageNetPrecision), 0);
        int xmax = std::min(int((pos.x + radius) * damageNetPrecision), enemyDamageNetModify->width());
        int ymax = std::min(int((pos.y + radius) * damageNetPrecision), enemyDamageNetModify->height());

        fillDamageModify(pos, radius, agent);

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
                damage.light += bonus.bonus / w.speed * 100;
            }
            else if (bonus.attribute == Attribute::Armored) {
                damage.armored += bonus.bonus / w.speed * 100;
            }
            else if (bonus.attribute == Attribute::Biological) {
                damage.biological += bonus.bonus / w.speed * 100;
            }
            else if (bonus.attribute == Attribute::Mechanical) {
                damage.mechanical += bonus.bonus / w.speed * 100;
            }
            else if (bonus.attribute == Attribute::Massive) {
                damage.massive += bonus.bonus / w.speed * 100;
            }
            else if (bonus.attribute == Attribute::Psionic) {
                damage.psionic += bonus.bonus / w.speed * 100;
            }
            else {
                printf("ATTRIBUTE NOT PROGRAMMED %d\n", bonus.attribute);
            }
        }

        if (w.type == Weapon::TargetType::Air || w.type == Weapon::TargetType::Any) {
            d.air.normal = w.damage_ / w.speed * 100;
            d.air += damage;
        }
        if (w.type == Weapon::TargetType::Ground || w.type == Weapon::TargetType::Any) {
            d.ground.normal = w.damage_ / w.speed * 100;
            d.ground += damage;
        }
        return d;
    }

    float getRelevantDamage(UnitWrapper* unitWrap, DamageLocation pointDamage, Agent* agent) {
        Composition comp = unitWrap->getComposition(agent);
        float damage = 0;
        if (comp == Composition::Ground || comp == Composition::Any) {
            damage += pointDamage.ground.normal;
            for (Attribute a : Aux::getStats(unitWrap->type, agent).attributes) {
                switch (a) {
                case(Attribute::Light): {
                    damage += pointDamage.ground.light;
                }
                case(Attribute::Armored): {
                    damage += pointDamage.ground.armored;
                }
                case(Attribute::Biological): {
                    damage += pointDamage.ground.biological;
                }
                case(Attribute::Mechanical): {
                    damage += pointDamage.ground.mechanical;
                }
                case(Attribute::Massive): {
                    damage += pointDamage.ground.massive;
                }
                case(Attribute::Psionic): {
                    damage += pointDamage.ground.psionic;
                }
                }
            }

        }
        if (comp == Composition::Air || comp == Composition::Any) {
            damage += pointDamage.air.normal;
            for (Attribute a : Aux::getStats(unitWrap->type, agent).attributes) {
                switch (a) {
                case(Attribute::Light): {
                    damage += pointDamage.air.light;
                }
                case(Attribute::Armored): {
                    damage += pointDamage.air.armored;
                }
                case(Attribute::Biological): {
                    damage += pointDamage.air.biological;
                }
                case(Attribute::Mechanical): {
                    damage += pointDamage.air.mechanical;
                }
                case(Attribute::Massive): {
                    damage += pointDamage.air.massive;
                }
                case(Attribute::Psionic): {
                    damage += pointDamage.air.psionic;
                }
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
        int x = (pos.x - radius) * damageNetPrecision;
        int y = (pos.y - radius) * damageNetPrecision;
        int xmax = (pos.x + radius) * damageNetPrecision;
        int ymax = (pos.y + radius) * damageNetPrecision;
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

    Point2D weightedVector(UnitWrapper* unitWrap, float radius, Agent* agent) {
        //printf("5ize:%d\n", sizeof(DamageLocation));
        enemyDamageNetTemp->clear();
        Point2D pos = unitWrap->pos(agent);
        int x = (pos.x - radius) * damageNetPrecision;
        int y = (pos.y - radius) * damageNetPrecision;
        int xmax = (pos.x + radius) * damageNetPrecision;
        int ymax = (pos.y + radius) * damageNetPrecision;
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
            return units.at(type);
        }
        return UnitWrappers();
    }

    UnitWrappers getMulti(vector<UnitTypeID> types) {
        int total = 0;
        for (UnitTypeID type : types) {
            if (checkExist(type)) {
                total += units.at(type).size();
            }
        }
        UnitWrappers wraps;
        wraps.reserve(total);
        for (UnitTypeID type : types) {
            if (checkExist(type)) {
                for (UnitWrapper* wrap : units.at(type)) {
                    wraps.push_back(wrap);
                }
            }
        }
        return wraps;
    }

    UnitWrapper* find(UnitTypeID type, Tag tag) {
        UnitWrappers v = get(type);
        for (int i = 0; i < v.size(); i++) {
            if (v.at(i)->self == tag) {
                return v.at(i);
            }
        }
        return nullptr;
    }

    bool checkExistNeutral(UnitTypeID id) {
        return neutrals.find(id) != neutrals.end();
    }

    UnitWrappers getNeutral(UnitTypeID type) {
        if (checkExistNeutral(type)) {
            return neutrals.at(type);
        }
        return UnitWrappers();
    }

    UnitWrappers getNeutralMulti(vector<UnitTypeID> types) {
        int total = 0;
        for (UnitTypeID type : types) {
            if (checkExistNeutral(type)) {
                total += neutrals.at(type).size();
            }
        }
        UnitWrappers wraps;
        wraps.reserve(total);
        for (UnitTypeID type : types) {
            if (checkExistNeutral(type)) {
                for (UnitWrapper* wrap : neutrals.at(type)) {
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
            if (v.at(i)->self == tag) {
                return v.at(i);
            }
        }
        return nullptr;
    }

    bool checkExistEnemy(UnitTypeID id) {
        return enemies.find(id) != enemies.end();
    }

    UnitWrappers getEnemy(UnitTypeID type) {
        if (checkExistEnemy(type)) {
            return enemies.at(type);
        }
        return UnitWrappers();
    }

    UnitWrappers getEnemyMulti(vector<UnitTypeID> types) {
        int total = 0;
        for (UnitTypeID type : types) {
            if (checkExistEnemy(type)) {
                total += enemies.at(type).size();
            }
        }
        UnitWrappers wraps;
        wraps.reserve(total);
        for (UnitTypeID type : types) {
            if (checkExistEnemy(type)) {
                for (UnitWrapper* wrap : enemies.at(type)) {
                    wraps.push_back(wrap);
                }
            }
        }
        return wraps;
    }

    UnitWrapper *findEnemy(UnitTypeID type, Tag tag) {
        UnitWrappers v = getEnemy(type);
        for (int i = 0; i < v.size(); i++) {
            if (v.at(i)->self == tag) {
                return v.at(i);
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
//    UnitManager::units.at(type).push_back(this);
//}

UnitWrapper::UnitWrapper(const Unit *unit) : self(unit->tag), type(unit->unit_type), lastPos{0, 0, 0}, radius(unit->radius), team(unit->alliance), isBuilding(unit->is_building), health(0), healthMax(0), shields(0), shieldsMax(0) {
    if (unit->alliance == Unit::Alliance::Self) {
        if (!UnitManager::checkExist(type)) {
            UnitManager::units[type] = UnitWrappers();
        }
        UnitManager::units.at(type).push_back(this);
        //if (std::find(UnitManager::units.at(type).begin(), UnitManager::units.at(type).end(), this) == UnitManager::units.at(type).end()) {
        //    UnitManager::units.at(type).push_back(this);
        //}
        //else {
        //    printf("DuplicateUnit\n");
        //}
    }else if (unit->alliance == Unit::Alliance::Neutral) {
        if (!UnitManager::checkExistNeutral(type)) {
            UnitManager::neutrals[type] = UnitWrappers();
        }
        UnitManager::neutrals.at(type).push_back(this);
        //if (std::find(UnitManager::neutrals.at(type).begin(), UnitManager::neutrals.at(type).end(), this) == UnitManager::neutrals.at(type).end()) {
        //    UnitManager::neutrals.at(type).push_back(this);
        //}
        //else {
        //    printf("DuplicateNeutral\n");
        //}
    }else if (unit->alliance == Unit::Alliance::Enemy) {
        if (!UnitManager::checkExistEnemy(type)) {
            UnitManager::enemies[type] = UnitWrappers();
        }
        //UnitManager::enemies.at(type).push_back(this);
        bool found = false;
        for (UnitWrapper* wrap : UnitManager::enemies.at(type)) {
            if (unit->tag == wrap->self) {
                found = true;
                //printf("DuplicateEnemy O:%s N:%s\n", UnitTypeToName(wrap->type), UnitTypeToName(unit->unit_type));
                break;
            }
        }
        if (found) {
            //printf("DuplicateEnemy\n", UnitTypeToName());
        } else {
            UnitManager::enemies.at(type).push_back(this);
        }
        //if (std::find(UnitManager::enemies.at(type).begin(), UnitManager::enemies.at(type).end(), this) == UnitManager::enemies.at(type).end()) {
        //    UnitManager::enemies.at(type).push_back(this);
        //}
        //else {
        //    printf("DuplicateEnemy\n");
        //}
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
    return agent->Observation()->GetUnit(self);
}

bool UnitWrapper::equals(UnitWrapper *wrapper) {
    return (wrapper->self == self) && (wrapper->type == type);
}

//We only care about nexus, probe, gateway, ?zealot?, forge, cybercore, sentry, stalker, adept, shield battery, twilight council, stargate, pheonix, oracle, void ray, robotics facility, observer, warp prism, templar, high templar, archon, dark shrine, dark templar, fleet beacon, ?carrier, mothership, robotics bay, disruptor
void UnitWrapper::loadAbilities(Agent *agent) {
    //Units all = agent->Observation()->GetUnits(Unit::Alliance::Self);
    //vector<AvailableAbilities> allAb = agent->Query()->GetAbilitiesForUnits(all);
    //for (AvailableAbilities abil : allAb) {
    //    if (abil.unit_tag != NullTag) {
    //        UnitWrapper* u = UnitManager::find(abil.unit_type_id, abil.unit_tag);
    //        if (u == nullptr) {
    //            continue;
    //        }
    //        u->abilities = abil;
    //    }
    //}
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
    for (AvailableAbilities abil : allAb) {
        if (abil.unit_tag != NullTag) {
            UnitWrapper* u = UnitManager::find(abil.unit_type_id, abil.unit_tag);
            if (u == nullptr) {
                continue;
            }
            u->abilities = abil;
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
        for (auto it = UnitManager::units.at(type).begin(); it != UnitManager::units.at(type).end(); it++) {
            if ((*it)->self == self) {
                UnitManager::units.at(type).erase(it);
                break;
            }
        }
    } else if (team == Unit::Alliance::Neutral) {
        for (auto it = UnitManager::neutrals.at(type).begin(); it != UnitManager::neutrals.at(type).end(); it++) {
            if ((*it)->self == self) {
                UnitManager::neutrals.at(type).erase(it);
                break;
            }
        }
    } else if (team == Unit::Alliance::Enemy) {
        for (auto it = UnitManager::enemies.at(type).begin(); it != UnitManager::enemies.at(type).end(); it++) {
            if ((*it)->self == self) {
                UnitManager::enemies.at(type).erase(it);
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