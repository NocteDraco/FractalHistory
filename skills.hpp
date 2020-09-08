#ifndef SKILLS_HPP
#define SKILLS_HPP

#include <stdlib.h>

int d20()
{
    return rand()%20 + 1;
}

int skillCheck(int skill)
{
    // d20 plus the skill modifier
    return d20() + skill / 2 - 5;
}

int profSkillCheck(int skill, int prof, int profMult = 1)
{
    // d20 plus skill modifier plus proficiency and multiplier
    return skillCheck(skill) + profMult * prof;
}

#endif