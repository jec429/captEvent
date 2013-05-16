#ifndef CaptGeomIdDef_hxx_seen
#define CaptGeomIdDef_hxx_seen

namespace CP {
    namespace GeomId {
        /// Define the fields used to create a unique identifier for elements
        /// in the geometry.  This is a 32 bit field with the bits number from
        /// 0 (least significant bit or LSB) to 31 (most significant bit or
        /// MSB).  The bit fields are defined as follows
        ///
        /// \code 
        /// r dddddd sssssssssssssssssssssssss
        /// r(1)  -- Reserved.  This is actually a "guard bit" that 
        ///          must be zero. 
        /// d(6)  -- The sub-detector identifier
        /// s(25) -- The sub-detector specific field
        /// \endcode
        ///
        /// The value of all constants and order of all enumerations defined
        /// below is significant and must not be changed.
        namespace Def {
#define MAKE_MASK(msb,lsb) (((1<<(((msb)-(lsb))+1))-1)<<(lsb))
        
            /// A bitwise mask for a field;
            typedef unsigned long GeomIdMask;
        
            /// Bit number.  The bits are counted 0 (LSB) to 31 (MSB)
            typedef unsigned long GeomIdBit;
        
            /// A value that can be used to initialize an empty geometry
            /// identifier.
            const GeomIdMask   kEmptyId = 0;

            /// The most significant bit of the reserved guard bit.
            const GeomIdBit   kReserved1MSB = 31;
            /// The least significant bit of the reserved guard bit.  This bit
            /// must be zero for a geometry Id.  The value of 1 is reserved
            /// for future use to indicate an electronics channel number.
            const GeomIdBit   kReserved1LSB = 31;
        
            /// The mask to select the first reserved field.  The value in the
            /// first reserved field must be zero.
            const GeomIdMask kReserved1Mask 
            = MAKE_MASK(kReserved1MSB,kReserved1LSB);

            /// The most significant bit of the detector identifier.  This is
            /// a 6 bit field.
            const GeomIdBit   kDetectorIdMSB = kReserved1LSB-1;

            /// The least significant bit of the detector identifier.
            const GeomIdBit   kDetectorIdLSB = kDetectorIdMSB-5;

            /// The mask to select the detector identifier
            const GeomIdMask kDetectorIdMask 
            = MAKE_MASK(kDetectorIdMSB,kDetectorIdLSB);

            /// Enumeration defining the detector identifiers.  The detector
            /// identifiers are used to uniquely define which sub-detector
            /// contains a geometry element.  All geometry elements are by
            /// definition within at most one sub-detector.  The identifiers
            /// are held in the bit field defined by DetectorMask.  The
            /// conversion from a sub-detector identifier to a field value is
            /// (value<<DetectorLSB).  The conversion from a field value to a
            /// sub-detector identifier is ((field&DetectorMask)>>DetectorLSB)
            enum DetectorId {
                kROOTGeoNodeId = 0,
                kCryostat,
            };

            /// The most significant bit in the field reserved to the
            /// sub-detector.
            const GeomIdBit kSubDetectorMSB = kDetectorIdLSB-1;
            /// The least significant bit in the field reserved to the
            /// sub-detector.
            const GeomIdBit kSubDetectorLSB = 0;
        
            /// The mask to select the sub-detector detector fields.
            const GeomIdMask kSubDetectorMask = MAKE_MASK(kSubDetectorMSB,
                                                          kSubDetectorLSB);
        
            /// Define the cryostat specific constants used to decode a geometry
            /// identifier.  The field is 25 bits divided into a sequence
            /// identifier, and a field value.
            ///
            /// \code
            /// sssss ffffffffffffffffffff
            /// s(4)  -- The sequence identifier (0 to 15)
            /// f(21) -- The field value associated with a sequence id.
            /// \endcode
            ///
            /// Depending on the particular sequence, the field values may be
            /// further sub-divided.
            namespace Captain {
                /// Define the sequences required in the CRYO.
                enum CaptainSeqId {
                    /// A global volume.  This is used for identifiers that
                    /// refer to volumes which contain other geometry
                    /// identifiers.
                    kGlobal = 0,
                    /// A wire.
                    kWire,
                };

                /// The most significant bit of the sequence identifier field.
                /// The sequence id field can have values between 0-15.
                const GeomIdBit kSeqIdMSB = Def::kSubDetectorMSB;
                const GeomIdBit kSeqIdLSB = kSeqIdMSB - 3;
                const GeomIdMask kSeqIdMask = MAKE_MASK(kSeqIdMSB,kSeqIdLSB);

                /// A field that is unique to each global volume.  A global
                /// volume is any volume (with a geometry identifier) that
                /// contains other volumes with geometry identifiers.
                /// Examples are the wire planes, and the drift region.  The
                /// bits are defined as
                ///
                /// \code
                /// gggggggg fffffffffff
                /// g(8)  -- The type of global volume.
                /// f(13) -- The field value.
                /// \endcode
                namespace Global {
                    /// @{ Define a field to hold the type of global volume
                    /// being identified.
                    const GeomIdBit kSeqIdMSB = Def::Captain::kSeqIdLSB-1;
                    const GeomIdBit kSeqIdLSB = kSeqIdMSB-7;
                    const GeomIdMask kSeqIdMask=MAKE_MASK(kSeqIdMSB,kSeqIdLSB);
                    /// @}
                
                    enum GlobalSeqId {
                        /// A special case for one of a kind global volumes
                        kCryostat,
                        /// The drift region
                        kDriftRegion,
                        /// A wire plane.
                        kWirePlane,
                        /// A light sensor (i.e a PMT)
                        kLightSensor,
                    };
                
                    /// @{ The sequence value for this type of global volume.
                    const GeomIdBit kFieldMSB = kSeqIdLSB-1;
                    const GeomIdBit kFieldLSB = 0;
                    const GeomIdMask kFieldMask=MAKE_MASK(kFieldMSB,kFieldLSB);
                    /// @}
                };

                /// A field that is unique for each wire.
                /// \code
                /// pppppppp wwwwwwwwwwwww
                /// p(8) -- The wire plane.
                /// w(13) -- The wire number.
                /// \endcode
                namespace Wire {
                    /// @{ The wire plane.  The 0) X Plane 1) V Plane 2) U
                    /// Plane.
                    const GeomIdBit kPlaneMSB = Def::Captain::kSeqIdLSB-1;
                    const GeomIdBit kPlaneLSB = kPlaneMSB-7;
                    const GeomIdMask kPlaneMask = MAKE_MASK(kPlaneMSB,
                                                              kPlaneLSB);
                    /// @}

                    /// @{ The wire within a plane.  The wires are numbered
                    /// starting with zero, and wire 0 is at the most negative
                    /// coordinate.
                    const GeomIdBit kWireMSB = kPlaneLSB-1;
                    const GeomIdBit kWireLSB = kWireMSB-12;
                    const GeomIdMask kWireMask=MAKE_MASK(kWireMSB,kWireLSB);
                    /// @}
                };            
            };

#undef MAKE_FIELD_MASK
        };
    };
};
#endif
