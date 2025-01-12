/* ------------------------------------------------------------------------- *
 *                                SPHinXsys                                  *
 * ------------------------------------------------------------------------- *
 * SPHinXsys (pronunciation: s'finksis) is an acronym from Smoothed Particle *
 * Hydrodynamics for industrial compleX systems. It provides C++ APIs for    *
 * physical accurate simulation and aims to model coupled industrial dynamic *
 * systems including fluid, solid, multi-body dynamics and beyond with SPH   *
 * (smoothed particle hydrodynamics), a meshless computational method using  *
 * particle discretization.                                                  *
 *                                                                           *
 * SPHinXsys is partially funded by German Research Foundation               *
 * (Deutsche Forschungsgemeinschaft) DFG HU1527/6-1, HU1527/10-1,            *
 *  HU1527/12-1 and HU1527/12-4.                                             *
 *                                                                           *
 * Portions copyright (c) 2017-2023 Technical University of Munich and       *
 * the authors' affiliations.                                                *
 *                                                                           *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may   *
 * not use this file except in compliance with the License. You may obtain a *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.        *
 *                                                                           *
 * ------------------------------------------------------------------------- */
/**
 * @file particle_generator_lattice.h
 * @brief The lattice generator generates particles
 * at lattice position by check whether the position is contained by a SPH body.
 * @author Chi Zhang and Xiangyu Hu
 */

#ifndef PARTICLE_GENERATOR_LATTICE_H
#define PARTICLE_GENERATOR_LATTICE_H

#include "base_particle_generator.h"

namespace SPH
{

class Shape;
class ParticleRefinementByShape;
class ShellParticles;

template <> // Base class for generating particles from lattice positions
class GeneratingMethod<Lattice>
{
  public:
    explicit GeneratingMethod(SPHBody &sph_body);
    virtual ~GeneratingMethod(){};

  protected:
    Real lattice_spacing_;      /**< Initial particle spacing. */
    BoundingBox domain_bounds_; /**< Domain bounds. */
    Shape &initial_shape_;         /**< Geometry shape for body. */
};

template <>
class ParticleGenerator<Lattice>
    : public ParticleGenerator<Base>, public GeneratingMethod<Lattice>
{
  public:
    explicit ParticleGenerator(SPHBody &sph_body);
    virtual ~ParticleGenerator(){};
    virtual void initializeGeometricVariables() override;
};
using ParticleGeneratorLattice = ParticleGenerator<Lattice>;

template <> // For generating particles with adaptive resolution from lattice positions
class ParticleGenerator<Lattice, Adaptive> : public ParticleGenerator<Lattice>
{
  public:
    ParticleGenerator(SPHBody &sph_body, Shape &target_shape);
    explicit ParticleGenerator(SPHBody &sph_body);
    virtual ~ParticleGenerator(){};

  protected:
    Shape &target_shape_;
    ParticleRefinementByShape *particle_adaptation_;
    virtual void initializePositionAndVolumetricMeasure(const Vecd &position, Real volume) override;
};
using ParticleGeneratorAdaptive = ParticleGenerator<Lattice, Adaptive>;

template <> // For generating surface particles from lattice positions using reduced order approach
class ParticleGenerator<Surface, Lattice, ReducedOrder>
    : public ParticleGenerator<Surface>, public GeneratingMethod<Lattice>
{
  public:
    ParticleGenerator(SPHBody &sph_body, Real thickness);
    virtual ~ParticleGenerator(){};
    virtual void initializeGeometricVariables() override;

  protected:
    Real total_volume_;                  /**< Total volume of body calculated from level set. */
    Real thickness_;                     /**< Global average thickness. */
    Real particle_spacing_;              /**< Particle spacing. */
    Real avg_particle_volume_;           /**< Average particle volume. */
    size_t all_cells_;                   /**< Number of cells enclosed by the volume. */
    size_t planned_number_of_particles_; /**< Number of particles in planned manner. */
};
using ParticleGeneratorThickSurface = ParticleGenerator<Surface, Lattice, ReducedOrder>;
} // namespace SPH
#endif // PARTICLE_GENERATOR_LATTICE_H
