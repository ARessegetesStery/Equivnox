#pragma once

// All Logging functions should be inlined as this header is included multiple times throughout the project

#include "CoreLog.h"

#include "Runtime/Geometry/Geometry.h"
#include "Runtime/Landscape/LandscapeInfo.h"

namespace EQX {


	inline void Print(const Face&);
	inline void Print(const SceneInfo&);
	inline void Print(const EntityInfo&);

	void Print(const Face& f)
	{
		cout << "| Face:" << endl;
		cout << "| Vertex 1: ";
		Print(f.L().pos);
		cout << "| Vertex 2: ";
		Print(f.M().pos);
		cout << "|_Vertex 3: ";
		Print(f.R().pos);
	}
	void Print(const SceneInfo& info)
	{
		Print("| SceneInfo:");
		cout << "| Name: ";
		Print(info.name);
		cout << "|_Scene ID: ";
		Print(info.sceneID);
	}

	void Print(const EntityInfo& info)
	{
		Print("| EntityInfo: ");
		cout << "| Name: ";
		Print(info.name);
		cout << "| EntityUID: ";
		Print(info.entityID);
		cout << "| ConfigUID: ";
		Print(info.configID);
		cout << "| with parent:" << endl;
		Print(info.parent);
	}

}
