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
        
        /// Define the CAPTAIN specific geometry identifiers.
        namespace Captain {

            /// Define a geometry identifier to the entire Cryostat.
            TGeometryId Detector();
        
            /// Check if the id is for the cryostat.
            bool IsCryostat(TGeometryId id);

            /// Get the geometry identifier for the drift region.
            TGeometryId Drift();

            /// Check if the id is for the drift region.
            bool IsDrift(TGeometryId i);

            /// Get the geometry identifier for a wire plane.
            TGeometryId Plane(int plane);

            /// Check if the id is for a plane.
            bool IsPlane(TGeometryId i);

            enum {
                kXPlane = 0,
                kVPlane = 1,
                kUPlane = 2,
            };

            /// Get the wire plane from the id.
            int GetPlane(TGeometryId i);

            /// Get the geometry identifier for a wire light sensor.
            TGeometryId Photosensor(int photoSensor);

            /// Check if the id is for a photo sensor.
            bool IsPhotosensor(TGeometryId i);

            /// Get the photo sensor number from the id.
            int GetPhotosensor(TGeometryId i);

            /// The geometry identifiers for wires
            TGeometryId Wire(int plane, int wire);

            /// Check if the id is for a wire.
            bool IsWire(TGeometryId id);

            /// Check if the id is for a x wire.
            bool IsXWire(TGeometryId id);

            /// Check if the id is for a v wire.
            bool IsVWire(TGeometryId id);

            /// Check if the id is for a u wire.
            bool IsUWire(TGeometryId id);

            /// Get the wire plane from an id for a wire.
            int GetWirePlane(TGeometryId id);

            /// Get the wire number from an id.
            int GetWireNumber(TGeometryId id);
        };
    };
};
#endif
