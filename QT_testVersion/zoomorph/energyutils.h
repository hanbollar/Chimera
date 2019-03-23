#ifndef ENERGYUTILS_H
#define ENERGYUTILS_H
#include "mesh.h"

namespace EnergyUtils {

    /**
     * @brief calcConfigEnergy -
     * calculates the total energy associated with the current configuration
     * to determine if it's optimal or not
     *
     * @param animalMesh - undeformed animal mesh
     *
     * @param baseMesh - undeformed base mesh
     *
     * @param phi - vector of configuration parameters that encodes how the
     *              animal and base mesh are deformed and positioned
     * @param animalW - how much the output energy should be penalized by any
     *                  deformation to the animal shape
     * @param baseW - how much the output energy should be penalized by any
     *                non-uniform scaling of the base shape
     * @param registrationW - how much the output energy should be penalized by
     *                        bad alignement between the two shapes
     * @param visualSalienceW - how much the output energy is affected by/by not
     *                        maintaining distinct sections of the animal shape
     * @param gashW - how much the output energy is affected by geometry
     *              intruding from the animal shape into restricted zones
     *
     * @return the associated energy
     */
    // TODO: add signature to phi.
    float calcConfigEnergy(Mesh* animalMesh, Mesh* baseMesh, phi, float animalW,
                           float baseW, float registrationW,
                           float visualSalienceW, float gashW);

    /**
     * @brief getAnimalShapeTerm
     * calculates the energy of the animal shape associated with this
     * configuration setup
     * E(phi_a) = D(phi_a)/ D_m + C(phi_a)
     * For animal shapes using LBS, define C(phi_a) in terms of handle positions
     * For animal shapes using FFD, C(phi_A) = 0
     *
     * @return E_animal
     */
    float getAnimalShapeTerm(float w);

    /**
     * @brief getBaseShapeTerm
     * calculates the energy added by any non-uniform scaling of the base shape
     * Sum of squared differences of all pairs of segment
     *
     * @return E_baseshape
     */
    float getBaseShapeTerm(float w);

    /**
     * @brief getRegistrationTerm
     * calculates the energy added by bad alignement between the two shapes
     *
     * @return E_rt
     */
    float getRegistrationTerm(float w);

    /**
     * @brief getVisualSalienceTerm
     * calculates the energy added by/by not maintaining distinct sections of
     * the animal shape
     *
     * @return E_salience
     */
    float getVisualSalienceTerm(float w);

    /**
     * @brief getGashTerm
     * calculates the energy added by geometry intruding from the animal shape
     * into restricted zones
     *
     * @return E_gash
     */
    float getGashTerm(float w);
}

#endif // ENERGYUTILS_H
