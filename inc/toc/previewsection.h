#ifndef TOC_PREVIEWSECTION_H
#define TOC_PREVIEWSECTION_H

/*****************************************************************************
**
**  Definition of toc preview section C interface
**
**  Creation date:    2016/08/18
**  Created by:       Ole Liabo
**  Correction date:  2017/11/27
**  Corrected by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2016 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "boxing/bool.h"

#ifdef __cplusplus
extern "C" {
#endif
    
struct mxml_node_s;

// Definition structure afs_toc_preview_section
typedef struct afs_toc_preview_section_s
{
    unsigned int id;
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    unsigned int rotation;

    int reference_count;
} afs_toc_preview_section;

afs_toc_preview_section *  afs_toc_preview_section_create(void);
afs_toc_preview_section *  afs_toc_preview_section_create2( unsigned int id, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int r );
void                       afs_toc_preview_section_init(afs_toc_preview_section * toc_preview_section, unsigned int id, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int r);
void                       afs_toc_preview_section_free(afs_toc_preview_section * toc_preview_section);

afs_toc_preview_section *  afs_toc_preview_section_clone(const afs_toc_preview_section * toc_preview_section);
afs_toc_preview_section *  afs_toc_preview_section_get_new_reference(afs_toc_preview_section * toc_preview_section);
DBOOL                      afs_toc_preview_section_equal(const afs_toc_preview_section * toc_preview_section1, const afs_toc_preview_section * toc_preview_section2);
DBOOL                      afs_toc_preview_section_is_valid(const afs_toc_preview_section * toc_preview_section);
unsigned int               afs_toc_preview_section_content_width(const afs_toc_preview_section * toc_preview_section);
unsigned int               afs_toc_preview_section_content_height(const afs_toc_preview_section * toc_preview_section);
DBOOL                      afs_toc_preview_section_is_overlap(const afs_toc_preview_section * toc_preview_section1, const afs_toc_preview_section * toc_preview_section2);

DBOOL                      afs_toc_preview_section_save_file(const afs_toc_preview_section * toc_preview_section, const char * file_name, DBOOL compact);
char *                     afs_toc_preview_section_save_string(const afs_toc_preview_section * toc_preview_section, DBOOL compact);
DBOOL                      afs_toc_preview_section_save_xml(const afs_toc_preview_section * toc_preview_section, struct mxml_node_s* out);

DBOOL                      afs_toc_preview_section_load_file(afs_toc_preview_section * toc_preview_section, const char * file_name);
DBOOL                      afs_toc_preview_section_load_string(afs_toc_preview_section * toc_preview_section, const char * in);
DBOOL                      afs_toc_preview_section_load_xml(afs_toc_preview_section * toc_preview_section, struct mxml_node_s* input_node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // TOC_PREVIEWSECTION_H
