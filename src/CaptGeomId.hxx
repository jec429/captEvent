#ifndef CaptGeomId_hxx_seen
#define CaptGeomId_hxx_seen

#include "TGeometryId.hxx"

namespace CP {

    /// Define the geometry identifiers for elements in the Capt geometry.
    namespace GeomId {

        /// Return the geometry id indication None.
        TGeometryId Empty();

        /// Get the sub-system type.
        int GetSubSystem(TGeometryId id);
        
        /// Define the P0D specific geometry identifiers.
        namespace Cryo {
            /// Define a geometry identifier to the entire Cryostat.
            TGeometryId Detector();
        
            /// Check if the id is for the cryostat.
            bool IsCryostat(TGeometryId id);

            /// Get the geometry identifier for the drift region.
            TGeometryId Drift();

            /// Get the geometry identifier for a wire plane.
            TGeometryId Plane(int plane);

            /// Get the wire plane from the id.
            int GetPlane(TGeometryId i);

            /// The geometry identifiers for wires
            TGeometryId Wire(int plane, int wire);

            /// Get the wire plane from an id for a wire.
            int GetWirePlane(TGeometryId id);

            /// Get the wire number from an id.
            int GetWireNumber(TGeometryId id);

        };

    };
};
#endif
