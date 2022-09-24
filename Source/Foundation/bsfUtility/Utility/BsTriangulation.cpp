//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Utility/BsTriangulation.h"
#include "Math/BsVector3.h"

// Third party
#include "TetGen/tetgen.h"

namespace bs
{
	TetrahedronVolume Triangulation::Tetrahedralize(const Vector<Vector3>& points)
	{
		TetrahedronVolume volume;
		if (points.size() < 4)
			return volume;

		tetgenio input;
		input.numberofpoints = (int)points.size();
		input.pointlist = new REAL[input.numberofpoints * 3]; // Must be allocated with "new" because TetGen deallocates it using "delete"
		for(UINT32 i = 0; i < (UINT32)points.size(); ++i)
		{
			input.pointlist[i * 3 + 0] = points[i].X;
			input.pointlist[i * 3 + 1] = points[i].Y;
			input.pointlist[i * 3 + 2] = points[i].Z;
		}

		tetgenbehavior options;
		options.neighout = 2; // Generate adjacency information between tets and outer faces
		options.facesout = 1; // Output face adjacency
		options.quiet = 1; // Don't print anything

		tetgenio output;
		::tetrahedralize(&options, &input, &output);

		UINT32 numTetrahedra = (UINT32)output.numberoftetrahedra;
		volume.Tetrahedra.resize(numTetrahedra);

		for (UINT32 i = 0; i < numTetrahedra; ++i)
		{
			memcpy(volume.Tetrahedra[i].Vertices, &output.tetrahedronlist[i * 4], sizeof(INT32) * 4);
			memcpy(volume.Tetrahedra[i].Neighbors, &output.neighborlist[i * 4], sizeof(INT32) * 4);
		}

		// Generate boundary faces
		UINT32 numFaces = (UINT32)output.numberoftrifaces;
		for (UINT32 i = 0; i < numFaces; ++i)
		{
			INT32 tetIdx = -1;
			if (output.adjtetlist[i * 2] == -1)
				tetIdx = output.adjtetlist[i * 2 + 1];
			else if (output.adjtetlist[i * 2 + 1] == -1)
				tetIdx = output.adjtetlist[i * 2];
			else // Not a boundary face
				continue;

			volume.OuterFaces.push_back(TetrahedronFace());
			TetrahedronFace& face = volume.OuterFaces.back();

			memcpy(face.Vertices, &output.trifacelist[i * 3], sizeof(INT32) * 3);
			face.Tetrahedron = tetIdx;
		}

		// Ensure that vertex at the specified location points a neighbor opposite to it
		for(UINT32 i = 0; i < numTetrahedra; ++i)
		{
			INT32 neighbors[4];
			memcpy(neighbors, volume.Tetrahedra[i].Neighbors, sizeof(INT32) * 4);

			for(UINT32 j = 0; j < 4; ++j)
			{
				INT32 vert = volume.Tetrahedra[i].Vertices[j];

				for (UINT32 k = 0; k < 4; ++k)
				{
					INT32 neighborIdx = neighbors[k];
					if (neighborIdx == -1)
						continue;

					Tetrahedron& neighbor = volume.Tetrahedra[neighborIdx];
					if (vert != neighbor.Vertices[0] && vert != neighbor.Vertices[1] &&
						vert != neighbor.Vertices[2] && vert != neighbor.Vertices[3])
					{
						volume.Tetrahedra[i].Neighbors[j] = neighborIdx;
						break;
					}
				}
			}
		}

		return volume;
	}
}
