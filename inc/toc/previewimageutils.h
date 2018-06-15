#ifndef TOC_PREVIEWIMAGEUTILS_H
#define TOC_PREVIEWIMAGEUTILS_H

/*****************************************************************************
**
**  Definition of toc preview image manipulation utils C interface
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

#include "toc/previewlayoutdefinition.h" 
#include "boxing/image8.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TOC_PREVIEW_IMAGE_UTIL_EXTRACT_ERROR (-1)

int toc_preview_image_util_extract( boxing_image8*** images, const afs_toc_preview_layout_definition * definition, const boxing_image8 * source );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // TOC_PREVIEWLAYOUTDEFINITION_H
