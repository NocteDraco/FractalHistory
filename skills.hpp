#ifndef SKILLS_HPP
#define SKILLS_HPP

#include <stdlib.h>

int d20()
{
    return rand()%20 + 1;
}

int skillCheck(int skill)
{
    return d20() + skill / 2 - 5;
}

int profSkillCheck(int skill, int prof, int profMult = 1)
{
    return skillCheck(skill) + profMult * prof;
}

#endif