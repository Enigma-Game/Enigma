/*
 * Copyright (C) 2021 Andreas Lochmann
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "lev/Proxy.hh"
#include "lev/RatingManager.hh"
#include "lev/ScoreManager.hh"

using namespace std;
using namespace enigma;

namespace enigma { namespace lev {

    lev::RatingManager *theRatingMgr;
    lev::ScoreManager *theScoreMgr;

    SearchCombination::SearchCombination() : searchText(std::string("")) {
        reset();
        is_fresh = true;
        theRatingMgr = lev::RatingManager::instance();
        theScoreMgr = lev::ScoreManager::instance();
    }

    void SearchCombination::reset() {
        setSearchText(std::string(""));
        int_min = 1;      int_max = 5;
        dex_min = 1;      dex_max = 5;
        pat_min = 1;      pat_max = 5;
        kno_min = 1;      kno_max = 6;
        spe_min = 1;      spe_max = 5;
        dif_min = 0;      dif_max = 100;
        avr_min = 0;      avr_max = 100;
        checkRatings = false;
        onlyUnsolvedEasy = false;
        onlyUnsolvedHard = false;
        onlyMainPacks = false;
        sortMethod = SC_SORT_NONE;
    }

    void SearchCombination::setSearchText(std::string text) {
        is_fresh = false;
        searchText = (LowerCaseString) (text);
        originalSearchText = text;
    }

    std::string SearchCombination::getSearchText() {
        return originalSearchText;
    }

    void SearchCombination::setValue(SCValueKey key, SCValueMinMax mm, short value) {
        is_fresh = false;
        switch(key) {
        case SC_INT: {
            if(mm == SC_MIN)
                int_min = ecl::Clamp<short>(value, 1, int_max);
            else
                int_max = ecl::Clamp<short>(value, int_min, 5);
            break; }
        case SC_DEX: {
            if(mm == SC_MIN)
                dex_min = ecl::Clamp<short>(value, 1, dex_max);
            else
                dex_max = ecl::Clamp<short>(value, dex_min, 5);
            break; }
        case SC_PAT: {
            if(mm == SC_MIN)
                pat_min = ecl::Clamp<short>(value, 1, pat_max);
            else
                pat_max = ecl::Clamp<short>(value, pat_min, 5);
            break; }
        case SC_KNO: {
            if(mm == SC_MIN)
                kno_min = ecl::Clamp<short>(value, 1, kno_max);
            else
                kno_max = ecl::Clamp<short>(value, kno_min, 6);
            break; }
        case SC_SPE: {
            if(mm == SC_MIN)
                spe_min = ecl::Clamp<short>(value, 1, spe_max);
            else
                spe_max = ecl::Clamp<short>(value, spe_min, 5);
            break; }
        case SC_DIF: {
            if(mm == SC_MIN)
                dif_min = ecl::Clamp<short>(value, 0, dif_max);
            else
                dif_max = ecl::Clamp<short>(value, dif_min, 100);
            break; }
        case SC_AVR: {
            if(mm == SC_MIN)
                avr_min = ecl::Clamp<short>(value, 0, avr_max);
            else
                avr_max = ecl::Clamp<short>(value, avr_min, 100);
            break; }
        default: {
            Log << "Warning: SearchCombination::setValue called with invalid key combination!\n";
            break; }
        }
    }

    short SearchCombination::getValue(SCValueKey key, SCValueMinMax mm) {
        switch(key) {
        case SC_INT: { return (mm == SC_MIN) ? int_min : int_max; break; }
        case SC_DEX: { return (mm == SC_MIN) ? dex_min : dex_max; break; }
        case SC_PAT: { return (mm == SC_MIN) ? pat_min : pat_max; break; }
        case SC_KNO: { return (mm == SC_MIN) ? kno_min : kno_max; break; }
        case SC_SPE: { return (mm == SC_MIN) ? spe_min : spe_max; break; }
        case SC_DIF: { return (mm == SC_MIN) ? dif_min : dif_max; break; }
        case SC_AVR: { return (mm == SC_MIN) ? avr_min : avr_max; break; }
        default: {
            Log << "Warning: SearchCombination::getValue called with invalid key combination!\n";
            break; }
        }
        return 0;
    }

    void SearchCombination::setOnlyUnsolved(Difficulty diff, bool value) {
        is_fresh = false;
        switch(diff) {
        case DIFFICULTY_EASY: { onlyUnsolvedEasy = value; break; }
        case DIFFICULTY_HARD: { onlyUnsolvedHard = value; break; }
        default: {
            Log << "Warning: SearchCombination::setOnlyUnsolved called with invalid key!\n";
            break; }
        }
    }

    bool SearchCombination::getOnlyUnsolved(Difficulty diff) {
        switch(diff) {
        case DIFFICULTY_EASY: { return onlyUnsolvedEasy; break; }
        case DIFFICULTY_HARD: { return onlyUnsolvedHard; break; }
        default: {
            Log << "Warning: SearchCombination::getOnlyUnsolved called with invalid key!\n";
            break; }
        }
        return false;
    }

    bool SearchCombination::toggleOnlyUnsolved(Difficulty diff) {
        is_fresh = false;
        setOnlyUnsolved(diff, !getOnlyUnsolved(diff));
        return getOnlyUnsolved(diff);
    }

    void SearchCombination::prepareForSearch() {
        checkRatings = !(   (int_min == 1) && (int_max == 5)
                         && (dex_min == 1) && (dex_max == 5)
                         && (pat_min == 1) && (pat_max == 5)
                         && (kno_min == 1) && (kno_max == 6)
                         && (spe_min == 1) && (spe_max == 5)
                         && (dif_min == 0) && (dif_max == 100)
                         && (avr_min == 0) && (avr_max == 100)
                         && (sortMethod != SC_SORT_DIF));
    }

    bool SearchCombination::fits(Proxy *p) {
        // All criteria have to fit.
        // Numerical criteria:
        if (checkRatings) {
            Rating * r = theRatingMgr->findRating(p);
            if (   (r == NULL)
                || !ecl::isOrdered(int_min, r->intelligence,  int_max)
                || !ecl::isOrdered(dex_min, r->dexterity,     dex_max)
                || !ecl::isOrdered(pat_min, r->patience,      pat_max)
                || !ecl::isOrdered(kno_min, r->knowledge,     kno_max)
                || !ecl::isOrdered(spe_min, r->speed,         spe_max)
                || !ecl::isOrdered(dif_min, r->difficulty(),  dif_max)
                || !ecl::isOrdered(avr_min, r->averageRating, avr_max))
                return false;
        }
        // Boolean criteria:
        if (onlyUnsolvedEasy && ((!p->hasEasyMode()) || theScoreMgr->isSolved(p, DIFFICULTY_EASY)))
            return false;
        if (onlyUnsolvedHard && theScoreMgr->isSolved(p, DIFFICULTY_HARD))
            return false;
        // TODO: if (!onlyMainPacks || ...)
        //    return false;
        // Criteria needed by sorting methods (for SC_SORT_DIF see prepareForSearch):
        if (   (sortMethod == SC_SORT_AVR)
            && (theRatingMgr->getDAverageRating(p) == -1))
            return false;
        // Textual criteria:
        return (   searchText.low.empty()
                || searchText.containedBy(p->getNormFilePath())
                || searchText.containedBy(p->getTitle())
                || searchText.containedBy(p->getId())
                || searchText.containedBy(p->getAuthor()));
    }

    SearchCombination* SearchCombination::theSingleton = 0;

    SearchCombination* SearchCombination::getSingleton() {
        if (theSingleton == 0) {
            theSingleton = new SearchCombination();
        }
        return theSingleton;
    }
}} // namespace enigma::lev
