#ifndef ENERGYUTILS_H
#define ENERGYUTILS_H


namespace EnergyUtils {

    /**
     * @brief calcConfigEnergy -
     * calculates the total energy associated with the current configuration
     * to determine if it's optimal or not
     *
     * @param phi - vector of configuration parameters to match infinite/finitie free/restricted qualifications
     * @param animalW - how much the output energy should be penalized by any deformation to the animal shape
     * @param baseW - how much the output energy should be penalized by any non-uniform scaling of the base shape
     * @param registrationW - how much the output energy should be penalized by bad alignement between the two shapes
     * @param visualSalienceW - how much the output energy is affected by/by not maintaining distinct sections of the animal shape
     * @param gashW - how much the output energy is affected by geometry intruding from the animal shape into restricted zones
     *
     * @return the associated energy
     */
    float calcConfigEnergy(phi, animalW, baseW, registrationW, visualSalienceW, gashW);

    /**
     * @brief getAnimalShapeTerm
     * calculates the energy of the animal shape associated with this configuration setup
     *
     * @return E_animal
     */
    float getAnimalShapeTerm(phi);

    /**
     * @brief getBaseShapeTerm
     * calculates the energy added by any non-uniform scaling of the base shape
     *
     * @return E_baseshape
     */
    float getBaseShapeTerm(phi);

    /**
     * @brief getRegistrationTerm
     * calculates the energy added by bad alignement between the two shapes
     *
     * @return E_rt
     */
    float getRegistrationTerm(phi);

    /**
     * @brief getVisualSalienceTerm
     * calculates the energy added by/by not maintaining distinct sections of the animal shape
     *
     * @return E_salience
     */
    float getVisualSalienceTerm(phi);

    /**
     * @brief getGashTerm
     * calculates the energy added by geometry intruding from the animal shape into restricted zones
     *
     * @return E_gash
     */
    float getGashTerm(phi);
}

#endif // ENERGYUTILS_H
