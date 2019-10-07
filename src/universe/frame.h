/*
 * frame.h - reference frame package
 *
 *  Created on: Oct 7, 2019
 *      Author: Tim Stark
 */

#pragma once

class Object;

namespace ofs::universe
{
//	class Object;
	class CelestialStar;
	class CelestialBody;

	class SystemTree
	{
	public:
		SystemTree(Object *object);
		~SystemTree() = default;

		inline CelestialStar *getStar() const { return starParent; }
		inline CelestialBody *getBody() const { return bodyParent; }
		inline int getSystemSize() const { return objects.size(); }

		inline bool isRoot() const { return bodyParent == nullptr; }

		void addObject(Object *object);

		Object *getObject(int idx) const;

	private:
		CelestialStar *starParent = nullptr;
		CelestialBody *bodyParent = nullptr;

		vector<Object *> objects;
	};

	class ReferenceFrame
	{
	public:
		ReferenceFrame() = default;
		~ReferenceFrame() = default;
	};
}
