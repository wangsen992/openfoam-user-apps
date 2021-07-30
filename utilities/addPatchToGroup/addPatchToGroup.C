/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2021 AUTHOR,AFFILIATION
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    addPatchToGroup

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "polyTopoChange.H"
#include "mapPolyMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
    // Loading mesh information
    const fvBoundaryMesh& boundaryMesh = mesh.boundary();
    const word oldInstance = mesh.pointsInstance();
    forAll(boundaryMesh, i)
    {
      const fvPatch& patch = *boundaryMesh.get(i);
      const word pname = patch.name();
      if (pname.starts_with("building") || 
          pname.starts_with("terrain"))
      {
        const polyPatch& ppatch = patch.patch();
        wordList& pGroups = const_cast<wordList&>(ppatch.inGroups());
        pGroups.append("viewFactorWall");
//        Info << pname << ": ";
//        Info<< ppatch.inGroups() << endl;
      }
    }

    // Some touches to indicate the mesh is updated.
    autoPtr<mapPolyMesh> map;
    polyTopoChange meshMod(mesh);
    map = meshMod.changeMesh(mesh, false);
    mesh.updateMesh(map());
    
    // Write new mesh
    ++runTime;
    Info << "Writing new mesh to time " << runTime.timeName() << endl;
    Info << "Write status: " << mesh.write() << endl;

    Info<< nl;
    runTime.printExecutionTime(Info);

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
