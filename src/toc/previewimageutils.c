/*****************************************************************************
**
**  Implementation of toc preview image manipulation utils C interface
**
**  Creation date:  2016/08/18
**  Created by:     Ole Liabo
**
**
**  Copyright (c) 2016 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

//  PROJECT INCLUDES
//
#include "toc/previewimageutils.h"


//  SYSTEM INCLUDES
//


/*! 
  * \addtogroup preview
  * \{
  */


//----------------------------------------------------------------------------
/*!
 *  \def TOC_PREVIEW_IMAGE_UTIL_EXTRACT_ERROR() previewimageutils.h
 *  \brief A macro for define extract error code.
 *
 *  A macro for define extract error code.
 *  Corresponds to a value "-1" of integer type.
 */


//---------------------------------------------------------------------------- 
/*! \brief Extract preview images from image
 * 
 *  Extract preview images from source image. Source image must be in logical 
 *  dimensions.
 *
 *  \param[out] images      Array of extracted images on success, else *images 
 *                          is set to NULL;
 *  \param[in]  definition  Layout definition to use.
 *  \param[in]  source      Source image in logical dimensions.
 *  \return                 Number of extracted images on success, otherwize 
 *                          TOC_PREVIEW_IMAGE_UTIL_EXTRACT_ERROR.
 */

int toc_preview_image_util_extract( boxing_image8*** images, const afs_toc_preview_layout_definition * definition, const boxing_image8 * source )
{
    if (NULL == definition || NULL == source)
    {
        return TOC_PREVIEW_IMAGE_UTIL_EXTRACT_ERROR;
    }
    
    const unsigned int previews_number = afs_toc_preview_layout_definition_get_section_count(definition);

    /*Is it correct to allocate memory here?*/
    if (previews_number > 0)
    {
        *images = (boxing_image8 **)malloc(sizeof(boxing_image8 *) * previews_number);
        if (*images == NULL)
        {
            return TOC_PREVIEW_IMAGE_UTIL_EXTRACT_ERROR;
        }
    }

    unsigned int extracted_images = 0;

    for (unsigned int i = 0; i < previews_number; ++i)
    {
        const afs_toc_preview_section * section = afs_toc_preview_layout_definition_get_section(definition, i);
        if (NULL != section &&
            (section->x + section->width) <= source->width &&
            (section->y + section->height) <= source->height)
        {
            boxing_image8 * cropped_image = boxing_image8_crop(source, section->x, section->y, section->width, section->height);
            (*images)[i] = boxing_image8_rotate(cropped_image, section->rotation);
            boxing_image8_free(cropped_image);
            ++extracted_images;
        }
        else
        {
            (*images)[i] = NULL;
        }
    }
    return extracted_images;
}


//----------------------------------------------------------------------------
/*!
  * \} end of preview group
  */
