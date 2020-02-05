/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2020, The Souffle Developers. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file AuxArityAnalysis.cpp
 *
 * Implementation of AST analyses classes
 *
 ***********************************************************************/

#include "AuxArityAnalysis.h"
#include "AstClause.h"
#include "AstProgram.h"
#include "AstRelation.h"
#include "AstRelationIdentifier.h"
#include "AstUtils.h"
#include "Global.h"
#include <string>

namespace souffle {

const size_t AuxiliaryArity::getEvaluationArity(const AstAtom* atom) const {
    if (atom->getName().getName().find("@delta_") == 0) {
        const AstRelationIdentifier& originalRel = AstRelationIdentifier(atom->getName().getName().substr(7));
        return getArity(program->getRelation(originalRel));
    } else if (atom->getName().getName().find("@new_") == 0) {
        const AstRelationIdentifier& originalRel = AstRelationIdentifier(atom->getName().getName().substr(5));
        return getArity(program->getRelation(originalRel));
    } else if (atom->getName().getName().find("@info_") == 0) {
        return 0;
    } else {
        return getArity(atom);
    }
}

const size_t AuxiliaryArity::computeArity(const AstRelation* relation) const {
    if (Global::config().has("provenance")) {
        if (Global::config().get("provenance") == "subtreeHeights") {
            size_t maxNrOfPremises = 0;
            for (auto& cur : relation->getClauses()) {
                size_t numberOfAtoms = cur->getAtoms().size();
                if (numberOfAtoms > maxNrOfPremises) {
                    maxNrOfPremises = numberOfAtoms;
                }
            }
            return maxNrOfPremises + 2;
        }
        return 2;
    } else {
        return 0;
    }
}

}  // end of namespace souffle
