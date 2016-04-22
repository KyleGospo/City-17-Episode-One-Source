/*
	Copyright (C) 2008 Seeing Machines Ltd. All rights reserved.

	This file is part of the FaceTrackingAPI, also referred to as "faceAPI".

	This file may be distributed under the terms of the Seeing Machines 
	FaceTrackingAPI Non-Commercial License Agreement.

	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

	Further information about faceAPI licensing is available at:
	http://www.seeingmachines.com/faceapi/licensing.html
*/
#ifndef SM_API_FACESEARCH_H
#define SM_API_FACESEARCH_H

/*! @file
    Defines functions that perform tracking in files. */
#ifdef __cplusplus
extern "C"
{
#endif

#define SM_API_FACE_SEARCH_UNKNOWN_NUM_FACES    -1  /*!< Used to represent an unknown number of faces */
#define SM_API_FACE_SEARCH_MAX_NUM_FACES       100  /*!< Maximum number of faces that can be searched for. */
#define SM_API_FACE_SEARCH_UNKNOWN_FACE_SIZE    -1  /*!< Used to represent an unknown face size */
#define SM_API_FACE_SEARCH_MIN_FACE_WIDTH_PIXELS 50 /*!< Minimum size of a faces in pixels that can be searched for. */
#define SM_API_FACE_SEARCH_UNKNOWN_FACE_ROT     -1  /*!< Used to represent an unknown face rotation angle */
#define SM_API_FACE_SEARCH_MAX_SEQUENCE_LENGTH  20  /*!< The maximum image sequence length that can be passed to smFaceSearch() */

/*! @brief Defines the intensity of the search routine, 
    with higher levels taking longer to process but delivering 
    more fine-grained information. */
typedef enum smFaceSearchLevel
{
    SM_API_FACE_SEARCH_LEVEL_0 = 0,     /*!< Approximately locates the pose of each face in the image sequence. */
    SM_API_FACE_SEARCH_LEVEL_1 = 1,     /*!< Locates facial landmarks in each face in the image sequence. */
    SM_API_FACE_SEARCH_LEVEL_2 = 2,     /*!< More accurately locates facial landmarks in each face in the image sequence. */
    SM_API_FACE_SEARCH_LEVEL_MAX = 2    /*!< Highest search level */
} smFaceSearchLevel;

/*! @brief Defines the high-level face-search algorithm configuration. */
typedef struct smFaceSearchConfig
{    
    int num_faces_hint;                 /*!< If the number of faces is known in advance, specify this value, otherwise set to @ref SM_API_FACE_SEARCH_UNKNOWN_NUM_FACES */
    smFaceSearchLevel level;            /*!< The search level to perform. Higher levels take longer.@see smFaceSearchLevel */
} smFaceSearchConfig;

/*! @brief Defines face-search algorithm geometry constraints using 2D pixel measurements. */
typedef struct smFaceSearch2DConstraints
{
	int min_face_width_pixels;          /*!< Minimum face size in pixels in the image. Tighter bounds will allow faster searching. Set to @ref SM_API_FACE_SEARCH_UNKNOWN_FACE_SIZE if not known. */
	int max_face_width_pixels;          /*!< Maximum face size in pixels in the image. Tighter bounds will allow faster searching. Set to @ref SM_API_FACE_SEARCH_UNKNOWN_FACE_SIZE if not known. */
} smFaceSearch2DConstraints;

/*! @brief Defines face-search algorithm geometry constraints using 3D world-coordinate measurements. */
typedef struct smFaceSearch3DConstraints
{
    float min_face_range_m;             /*!< Minimum face range in meters from the camera. Tighter bounds will allow faster searching. Set to @ref SM_API_FACE_SEARCH_UNKNOWN_FACE_SIZE if not known. */
    float max_face_range_m;             /*!< Maximum face range in meters from the camera. Tighter bounds will allow faster searching. Set to @ref SM_API_FACE_SEARCH_UNKNOWN_FACE_SIZE if not known. */
} smFaceSearch3DConstraints;

/*! @brief Defines detailed face-search algorithm settings */
typedef struct smFaceSearchSettings
{
    smCameraLensParams lens_params;         /*!< Lens coefficients for the camera that generated the image sequence. @see smCameraLensParamsFromHFOV() and smCameraLoadLensParamsFile() */
    smFaceSearch2DConstraints *c2d;         /*!< Optional 2D constraints. Set to 0 if unknown. */
    smFaceSearch3DConstraints *c3d;         /*!< Optional 3D constraints. Set to 0 if unknown. If both 2D and 3D constraints are specified, the 3D constraints are used in preference. */
	double max_rot_z_angle_deg;             /*!< Maximum +/- rotation angle around the z-axis (nose-axis) for a face. Set to @ref SM_API_FACE_SEARCH_UNKNOWN_FACE_ROT if not known. Smaller values yield faster results. */
} smFaceSearchSettings;

/*! @brief Defines search result information for a single face. */
typedef struct smFaceSearchResult
{
    int image_index;                    /*!< The index of the image in the sequence where the face was located. */
    smEngineHeadPoseData head_pose;     /*!< The position and orientation of the head in 3D. */
    smEngineFaceData face_data;         /*!< Face landmark locations in face-coordinates and optional face texture for the face. */
} smFaceSearchResult;

/*! @brief Defines search result information for multiple faces. */
typedef struct smFaceSearchResultList
{
    int num_faces_found;                /*!< The number of faces found by the search, set by the smFaceSearch() routine. Can be 0. */
    smFaceSearchResult *result;         /*!< Array of smFaceSearchResult set by the smFaceSearch() routine. There is one result for each face. */
    void *__vec;                        /*!< Internal use only. */
} smFaceSearchResultList;

/*! @brief Search for faces in an image sequence. 

    This function searches a set of images that are consecutive in time for faces.
    
	The function is able to search on several levels of refinement, see smFaceSearchLevel.
    Each level greatly alters performance. Level-0 is generally fairly fast, but level-1+ are much slower.\n
    - Level-0 runs a fast face detector that can locate a face region and approximate the size and orentiation of that region
    and provides a rough 3D solution.
    - Level-1 runs an algorithm that attempts to locate the interior facial landmarks of each face found by level-0. If the 
      landmarks cannot be located, the face is not "found". Therefore level-1 often finds less faces than level-0, but if a
      face is found, it is found accurately.
    - Level-2 is a more precise (and slower) version of Level-1.

    @param image_sequence Must point to an existing array of @a image_sequence_size smImageHandle handles. All images must be the same resoluton. See smImageCreateFromPNG() and smImageCreateFromInfo() 
    @param image_sequence_size Length of the @a image_sequence array. An array containing a single image is valid.
    @param config General configuration settings for the face-search, cannot be 0.
    @param settings Optional detailed configuration settings for the face-search. Can be 0. Specify these values to constrain the search-space of the algorithm and improve performance (recommended).
    @param face_results Must point to an existing smFaceSearchResultList. The contents of this structure are assigned by the routine.
           If no faces were found, the @a num_faces_found field is set to 0 and the @a result array is also set to 0.
    
    @note 
    - If faces were found, when you are done with the list of results, you must call smFaceSearchDestroyResults() to free internal resources.
    - @a face_results will not contain face texture images or face texture coordinate data. I.e. for each smFaceSearchResult, 
        smEngineFaceData::texture == 0, and smFaceLandmark::ftc == {0,0,0}.
	
	@see
	- smFaceSearchLevel
	- smImageCreateFromPNG()
	- smImageCreateFromInfo() */
SM_API(smReturnCode) smFaceSearch(const smImageHandle *image_sequence,
                                  int image_sequence_size,
                                  const smFaceSearchConfig *config,
                                  const smFaceSearchSettings *settings,
                                  smFaceSearchResultList *face_results);

/*! @brief Frees memory allocated by a call to smFaceSearch()
    @param face_results must have been passed into a successful call to smFaceSearch() before being passed to this routine.
    @return SM_API_OK if the results were destroyed successfully. */
SM_API(smReturnCode) smFaceSearchDestroyResults(smFaceSearchResultList *face_results);

#ifdef __cplusplus
}
#endif
#endif


