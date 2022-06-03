/* -------------------------------------------------------------------------*
 *								SPHinXsys									*
 * --------------------------------------------------------------------------*
 * SPHinXsys (pronunciation: s'finksis) is an acronym from Smoothed Particle	*
 * Hydrodynamics for industrial compleX systems. It provides C++ APIs for	*
 * physical accurate simulation and aims to model coupled industrial dynamic *
 * systems including fluid, solid, multi-body dynamics and beyond with SPH	*
 * (smoothed particle hydrodynamics), a meshless computational method using	*
 * particle discretization.													*
 *																			*
 * SPHinXsys is partially funded by German Research Foundation				*
 * (Deutsche Forschungsgemeinschaft) DFG HU1527/6-1, HU1527/10-1				*
 * and HU1527/12-1.															*
 *                                                                           *
 * Portions copyright (c) 2017-2020 Technical University of Munich and		*
 * the authors' affiliations.												*
 *                                                                           *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may   *
 * not use this file except in compliance with the License. You may obtain a *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.        *
 *                                                                           *
 * --------------------------------------------------------------------------*/
/**
* @file complex_shape.h
* @brief Here, we define the a container of different type of shapes, which allow
* @details the boolean operation between the different type of shapes. The shapes
		   can be defined previously and add to this complex shapes container.
* @author	Yongchuan Yu and Xiangyu Hu
*/

#ifndef COMPLEX_SHAPE_H
#define COMPLEX_SHAPE_H

#include "base_geometry.h"
#include <string>
namespace SPH
{
	class GeometryShape;
	class LevelSetShape;

	class ComplexShape : public BinaryShapes
	{
	public:
		explicit ComplexShape(const std::string &shape_name) : BinaryShapes(shape_name){};
		virtual ~ComplexShape(){};

		template <typename... ConstructorArgs>
		LevelSetShape *defineLevelSetShape(SPHBody *sph_body, const std::string &shape_name, ConstructorArgs &&...args)
		{
			size_t index = getShapeIndexByName(shape_name);
			LevelSetShape *level_set_shape = shapes_ptr_keeper_[index].createPtr<LevelSetShape>(
				sph_body, *shapes_and_ops_[index].first, std::forward<ConstructorArgs>(args)...);
			shapes_and_ops_[index].first = level_set_shape;
			return level_set_shape;
		};
	};

	using DefaultShape = ComplexShape;
}

#endif // COMPLEX_SHAPE_H