#ifndef _TACTO_HELPERS
#define _TACTO_HELPERS

/**
 * \namespace TactosonixHelpers
 *
 * \brief These are some general value definitions and helper functions.
 *
 * \author Bruno Angeles (bruno.angeles@mail.mcgill.ca)
 *
 * \version 1.0
 *
 * \date 2012/08/14
 *
 */

#define OFX_POT_NUMSEQUENCERSTEPS 16
#define DRAG_THRESHOLD 0.8f
#define RADIUS_LOOP_PCT 0.5f

namespace TactoHelpers
{
	/// Polar coordinates.
	struct polarCoords {
		float magnitude; ///< Pixels
		float angle; ///< Degrees
	};
	
	/// Cartesian coordinates.
	struct cartesianCoords {
		float x; ///< Pixels
		float y; ///< Pixels
	};

	/** \brief A function that converts polar coordinates to cartesian coordinates.
	* \param _magnitude The magnitude of the polar coordinates.
	* \param _angleRads The angle in radians of the polar coordinates.
	*/
	static cartesianCoords polToCar(float _magnitude, float _angleRads)
	{
		cartesianCoords retStruct;
		retStruct.x = _magnitude*cos(_angleRads);
		retStruct.y = _magnitude*sin(_angleRads);
		return retStruct;
	}

	/// Different types of musical loops.
	enum TACTO_LOOPTYPE {
		TACTO_LOOPTYPE_NONE, ///< No sound.
		TACTO_LOOPTYPE_DRUMS, ///< Percussions
		TACTO_LOOPTYPE_BASS, ///< Bass
		TACTO_LOOPTYPE_LEAD, ///< Lead instrument or voice
	};
};

#endif
