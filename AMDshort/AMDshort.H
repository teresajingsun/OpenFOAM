/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2019-2021 OpenCFD Ltd.
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

Class
    Foam::LESModels::AMDshort

Group
    grpLESTurbulence

Description
    The AMDshort SGS model.

    Reference:
    \verbatim
        AMDshort, J. (1963).
        General circulation experiments with the primitive equations: I.
        The basic experiment*.
        Monthly weather review, 91(3), 99-164.
    \endverbatim

    The form of the AMDshort model implemented is obtained from the
    k-equation model assuming local equilibrium which provides estimates of both
    k and epsilon separate from the sub-grid scale viscosity:

    \verbatim
        B = 2/3*k*I - 2*nuSgs*dev(D)

    where

        D = symm(grad(U));
        k from D:B + Ce*k^3/2/delta = 0
        nuSgs = Ck*sqrt(k)*delta
    \endverbatim

    The default model coefficients are
    \verbatim
        AMDshortCoeffs
        {
            Ck                  0.094;
            Ce                  1.048;
        }
    \endverbatim

See also
    Foam::LESModels::kEqn

SourceFiles
    AMDshort.C

\*---------------------------------------------------------------------------*/

#ifndef AMDshort_H
#define AMDshort_H

#include "LESModel.H"
#include "LESeddyViscosity.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace LESModels
{

/*---------------------------------------------------------------------------*\
                           Class AMDshort Declaration
\*---------------------------------------------------------------------------*/

template<class BasicTurbulenceModel>
class AMDshort
:
    public LESeddyViscosity<BasicTurbulenceModel>
{
    // Private Member Functions

        //- No copy construct
        AMDshort(const AMDshort&) = delete;

        //- No copy assignment
        void operator=(const AMDshort&) = delete;

protected:

    // Protected data

        dimensionedScalar Ck_;

        volTensorField filter_;


    // Protected Member Functions

        //- Return SGS kinetic energy
        //  calculated from the given velocity gradient
        tmp<volScalarField> k(const tmp<volTensorField>& gradU) const;

        //- Update the SGS eddy viscosity
        virtual void correctNut();

        void calcFilter();

public:

    typedef typename BasicTurbulenceModel::alphaField alphaField;
    typedef typename BasicTurbulenceModel::rhoField rhoField;
    typedef typename BasicTurbulenceModel::transportModel transportModel;


    //- Runtime type information
    TypeName("AMDshort");


    // Constructors

        //- Construct from components
        AMDshort
        (
            const alphaField& alpha,
            const rhoField& rho,
            const volVectorField& U,
            const surfaceScalarField& alphaRhoPhi,
            const surfaceScalarField& phi,
            const transportModel& transport,
            const word& propertiesName = turbulenceModel::propertiesName,
            const word& type = typeName
        );


    //- Destructor
    virtual ~AMDshort() = default;

        //- Read model coefficients if they have changed
        virtual bool read();

        //- Return SGS kinetic energy
        
	virtual tmp<volScalarField> k() const
        {
            return k(fvc::grad(this->U_));
        }

        //- Correct Eddy-Viscosity and related properties
        virtual void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LESModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "AMDshort.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
