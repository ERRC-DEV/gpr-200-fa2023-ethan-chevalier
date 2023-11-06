#pragma once
#include "procGen.h"

namespace ethanShader {

	ew::MeshData createPlane(float width, float height, int subdivisions)
	{
		ew::MeshData mesh;

		//Conjuring vertices from the ether
		for (int row = 0; row <= subdivisions; row++)
		{
			for (int col = 0; col <= subdivisions; col++)
			{
				ew::Vertex v;
				v.pos.x = width * (static_cast<float>(col) / static_cast<float>(subdivisions));
				v.pos.z = -height * (static_cast<float>(row) / static_cast<float>(subdivisions));
				v.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
				v.uv = ew::Vec2(static_cast<float>(col) / static_cast<float>(subdivisions), static_cast<float>(row) / static_cast<float>(subdivisions));
				mesh.vertices.push_back(v);
			}

		}
		//Conjuring indeces from the ether
		int columns = subdivisions + 1;
		for (int row = 0; row < subdivisions; row++)
		{
			for (int col = 0; col < subdivisions; col++)
			{
				int start = row * columns + col;
				//Bottom right triangle
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);
				//Top left triangle
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
			}
		}

		return mesh;
	}

	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData mesh;

		//Vertices
		float thetaStep = 2.0f * ew::PI / static_cast<float>(numSegments);
		float phiStep = ew::PI / numSegments;
		for (int row = 0; row <= numSegments; row++)
		{
			//First and last row converge at poles
			float phi = static_cast<float>(row) * phiStep;
			for (int col = 0; col <= numSegments; col++) //Duplicate column for each row
			{
				float theta = static_cast<float>(col) * thetaStep;
				ew::Vertex v;
				v.pos.x = radius * std::cos(theta) * std::sin(phi);
				v.pos.y = radius * std::cos(phi);
				v.pos.z = radius * std::sin(theta) * std::sin(phi);
				v.normal = ew::Normalize(v.pos);
				v.uv = ew::Vec2(static_cast<float>(col) / static_cast<float>(numSegments), static_cast<float>(row) / static_cast<float>(numSegments));
				mesh.vertices.push_back(v);
			}
		}

		//Indices
		int columns = numSegments + 1;
		//Skip top and bottom poles
		for (int row = 0; row < numSegments; row++)
		{
			if (row == 0)
			{
				int poleStart = row * numSegments;//Index of first pole vertex
				int	sideStart = numSegments + 1;
				for (int i = 0; i < numSegments; i++)
				{
					mesh.indices.push_back(sideStart + i);
					mesh.indices.push_back(poleStart + i); //Pole
					mesh.indices.push_back(sideStart + i + 1);
				}
			}
			else if (row == numSegments - 1)
			{
				int poleStart = (row + 2) * numSegments;//Index of first pole vertex
				int	sideStart = poleStart - numSegments - 1;// - numSegments;
				for (int i = 0; i < numSegments; i++)
				{
					mesh.indices.push_back(sideStart + i + 1);
					mesh.indices.push_back(poleStart + i); //Pole
					mesh.indices.push_back(sideStart + i);
				}
			}
			else
			{
				for (int col = 0; col < numSegments; col++)
				{
					int start = row * columns + col;
					//Triangle 1
					mesh.indices.push_back(start);
					mesh.indices.push_back(start + 1);
					mesh.indices.push_back(start + columns);
					//Triangle 2
					mesh.indices.push_back(start + columns + 1);
					mesh.indices.push_back(start + columns);
					mesh.indices.push_back(start + 1);
				}
			}
		}

		return mesh;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData mesh;

		//Vertices
		float topY = height / 2.0f; //y=0 is centered
		float bottomY = -topY;
		//Top center
		ew::Vertex topCenter;
		topCenter.pos = ew::Vec3(0, topY, 0);
		topCenter.normal = ew::Vec3(0, 1, 0);
		topCenter.uv = ew::Vec2(0.5f, 0.5f);
		//Conjure top center
		mesh.vertices.push_back(topCenter);
		//Bottom center create
		ew::Vertex bottomCenter;
		bottomCenter.pos = ew::Vec3(0, bottomY, 0);
		bottomCenter.normal = ew::Vec3(0, 1, 0);
		bottomCenter.uv = ew::Vec2(0.5, 0.5);
		//Top ring
		//Add top ring vertices (twice)
		float thetaStep = 2.0 * ew::PI / numSegments;
		for (int col = 0; col <= numSegments; col++)
		{
			ew::Vertex v;
			float theta = static_cast<float>(col) * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY; //or bottomY
			v.normal = ew::Vec3(0, 1, 0);
			v.uv = ew::Vec2(static_cast<float>(cos(theta) * 0.5f + 0.5f), static_cast<float>(sin(theta) * 0.5f + 0.5f));
			mesh.vertices.push_back(v);

		}
		for (int col = 0; col <= numSegments; col++)
		{
			ew::Vertex v;
			float theta = static_cast<float>(col) * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY; //or bottomY
			ew::Vec3 directionVector = (v.pos) - (topCenter.pos);
			v.normal = ew::Normalize(directionVector);
			v.uv = ew::Vec2(1 / static_cast<float>(numSegments) * static_cast<float>(col), 1);
			mesh.vertices.push_back(v);

		}
		//Bottom ring
		//Add bottom ring vertices (twice)
		for (int col = 0; col <= numSegments; col++)
		{
			ew::Vertex v;
			float theta = static_cast<float>(col) * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			ew::Vec3 directionVector = (v.pos) - (bottomCenter.pos);
			v.normal = ew::Normalize(directionVector);
			v.uv = ew::Vec2(1 / static_cast<float>(numSegments) * static_cast<float>(col), 0);
			mesh.vertices.push_back(v);

		}
		for (int col = 0; col <= numSegments; col++)
		{
			ew::Vertex v;
			float theta = static_cast<float>(col) * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY; //or bottomY
			v.normal = ew::Vec3(0, -1, 0);
			v.uv = ew::Vec2(static_cast<float>(cos(theta) * 0.5f + 0.5f), static_cast<float>(sin(theta) * 0.5f + 0.5f));
			mesh.vertices.push_back(v);

		}
		//Bottom center push		
		mesh.vertices.push_back(bottomCenter);



		//Indices
		//top cap
		int start = 1;//Index of first ring vertex
		int center = 0;//Index of center vertex
		for (int col = 0; col <= numSegments; col++)
		{
			mesh.indices.push_back(start + col);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + col + 1);
		}

		//bottom cap
		start = (numSegments*3)+4;//Index of first ring vertex
		center = (numSegments*4)+5;//Index of center vertex
		for (int col = 0; col <= numSegments; col++)
		{
			mesh.indices.push_back(start + col + 1);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + col);
		}

		int sideStart = numSegments;//index of first top ring vertex
		int columns = numSegments + 1;
		for (int col = 0; col <= columns; col++)
		{
			start = sideStart + col;
			//Triangle 1
			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns);
			//Triangle 2
			mesh.indices.push_back(start + columns + 1);
			mesh.indices.push_back(start + columns);
			mesh.indices.push_back(start + 1);
		}

		return mesh;
	}
}