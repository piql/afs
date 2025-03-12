#ifndef TOC_PREVIEWSECTIONS_H
#define TOC_PREVIEWSECTIONS_H

/*****************************************************************************
**
**  Definition of toc preview section C interface
**
**  Creation date:    2017/12/01
**  Created by:       Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "toc/previewsection.h"
#include "gvector.h"
#include "boxing/bool.h"
#include "mxml.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef gvector afs_toc_preview_sections_vector;

// Definition structure afs_toc_preview_sections
typedef struct afs_toc_preview_sections_s
{
    afs_toc_preview_sections_vector * sections;

    int reference_count;
} afs_toc_preview_sections;

afs_toc_preview_sections * afs_toc_preview_sections_create();
afs_toc_preview_sections * afs_toc_preview_sections_create2(mxml_node_t * sections_node);

void                       afs_toc_preview_sections_free(afs_toc_preview_sections * toc_preview_sections);

afs_toc_preview_sections * afs_toc_preview_sections_clone(const afs_toc_preview_sections * toc_preview_sections);
afs_toc_preview_sections * afs_toc_preview_sections_get_new_reference(afs_toc_preview_sections * toc_preview_sections);
DBOOL                      afs_toc_preview_sections_equal(const afs_toc_preview_sections * toc_preview_sections1, const afs_toc_preview_sections * toc_preview_sections2);

DBOOL                      afs_toc_preview_sections_is_valid(const afs_toc_preview_sections * toc_preview_sections);
unsigned int               afs_toc_preview_sections_size(const afs_toc_preview_sections * toc_preview_sections);
DBOOL                      afs_toc_preview_sections_append(afs_toc_preview_sections * toc_preview_sections, afs_toc_preview_section * toc_preview_section);
afs_toc_preview_section *  afs_toc_preview_sections_get(const afs_toc_preview_sections * toc_preview_sections, unsigned int index);

DBOOL                      afs_toc_preview_sections_save_file(const afs_toc_preview_sections * toc_preview_sections, const char * file_name, DBOOL compact);
char *                     afs_toc_preview_sections_save_string(const afs_toc_preview_sections * toc_preview_sections, DBOOL compact);
DBOOL                      afs_toc_preview_sections_save_xml(const afs_toc_preview_sections * toc_preview_sections, mxml_node_t* out);

DBOOL                      afs_toc_preview_sections_load_file(afs_toc_preview_sections * toc_preview_sections, const char * file_name);
DBOOL                      afs_toc_preview_sections_load_string(afs_toc_preview_sections * toc_preview_sections, const char * in);
DBOOL                      afs_toc_preview_sections_load_xml(afs_toc_preview_sections * toc_preview_sections, mxml_node_t* input_node);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // TOC_PREVIEWSECTIONS_H
