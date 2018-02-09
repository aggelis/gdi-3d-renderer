#pragma once

#include "SceneObject.h"
#include "Camera.h"
#include "Renderer.h"

#include <vector>

namespace GL {

	using namespace System;
	using namespace System::Drawing;

	// The Scene class represents a 3D scene that consists of SceneObjects.
	class Scene
	{
	public:
		Scene();
		void renderScene(Renderer ^renderer);

		// methods to manipulate objects;

		void setObjectPosition(int x_coord, int y_coord, int z_coord);
		void setObjectRotation(float x_angle, float y_angle, float z_angle);
		void setObjectReflection(bool xy, bool xz, bool yz);
		void setObjectScale(int x_scale, int y_scale, int z_scale);

		Vector3 getObjectPosition(bool worldCoords) const;
		Vector3 getObjectRotation(bool worldCoords) const;
		Vector3 getObjectReflection() const;
		Vector3 getObjectScale(bool worldCoords) const;

		Vector3 getCameraPosition(bool worldCoords) const;
		Vector3 getCameraRotation(bool worldCoords) const;

		void resetObject();
		void deleteObject();
		void selectNextObject();
		void selectPreviousObject();

		// methods to manipulate camera:

		void setCameraPosition(int x_coord, int y_coord, int z_coord);
		void setCameraRotation(float pitch, float yawn, float roll);
		void resetCamera();

		// Sets projection mode: perspective if the parameter is true, orthographics otherwise.
		void setProjectionMode(bool perspective);
		// Sets drawing mode flags: wireframe and/or solid.
		void setDrawingMode(bool wireframe, bool solid);
		void setCulling(bool cull);
		// Returns true is the scene contains no objects.
		bool isEmpty();
		bool isSelectedFirst();
		bool isSelectedLast();
		bool isPerspective();
		bool isWireframeMode();
		bool isSolidMode();
		bool isCulling();
		unsigned int objectCount();
		unsigned int getSelected();
		bool fromFile(String ^file);

		// test
		void addCube();
		void addPolygon();
		void addPolygons();

	private:
		void addObject(SceneObject &obj);
		Camera camera;
		std::vector<SceneObject> sceneObjects;
		int selectedObject;
		bool perspective;
		bool faceCull;
		bool drawWireframe;
		bool drawSolid;
	};
}