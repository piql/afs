#ifndef AFS_TOC_FILE_PREVIEW_PAGE_H
#define AFS_TOC_FILE_PREVIEW_PAGE_H

/*****************************************************************************
**
**  Definition of the toc file preview page interface
**
**  Creation date:  2017/04/10
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "boxing/bool.h"
#include "toc/previewlayoutdefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

// Definition structure afs_toc_file_preview_page
typedef struct afs_toc_file_preview_page_s
{
    char *        layout_id;
    unsigned int  start_frame;
    unsigned int  start_section;
    unsigned int  section_count;
    unsigned int  dimension_x;
    unsigned int  dimension_y;
    unsigned int  overlap_x;
    unsigned int  overlap_y;

    int reference_count;
} afs_toc_file_preview_page;


afs_toc_file_preview_page * afs_toc_file_preview_page_create();
afs_toc_file_preview_page * afs_toc_file_preview_page_create2(
    const char *  layout_id,
    unsigned int  start_frame,
    unsigned int  start_section,
    unsigned int  section_count,
    unsigned int  dimension_x,
    unsigned int  dimension_y,
    unsigned int  overlap_x,
    unsigned int  overlap_y);

void  afs_toc_file_preview_page_init(afs_toc_file_preview_page * toc_file_preview_page);
void  afs_toc_file_preview_page_init2(
    afs_toc_file_preview_page * toc_file_preview_page,
    const char *  layout_id,
    unsigned int  start_frame,
    unsigned int  start_section,
    unsigned int  section_count,
    unsigned int  dimension_x,
    unsigned int  dimension_y,
    unsigned int  overlap_x,
    unsigned int  overlap_y);

void  afs_toc_file_preview_page_free(afs_toc_file_preview_page * toc_file_preview_page);

afs_toc_file_preview_page * afs_toc_file_preview_page_clone(afs_toc_file_preview_page * toc_file_preview_page);
afs_toc_file_preview_page * afs_toc_file_preview_page_get_new_reference(afs_toc_file_preview_page * toc_file_preview_page);
DBOOL                       afs_toc_file_preview_page_equal(afs_toc_file_preview_page * toc_file_preview_page1, afs_toc_file_preview_page * toc_file_preview_page2);
DBOOL                       afs_toc_file_preview_page_is_valid(afs_toc_file_preview_page * toc_file_preview_page);

DBOOL                       afs_toc_file_preview_page_get_frames_count(afs_toc_file_preview_page * toc_file_preview_page, unsigned int * frames_count, const afs_toc_preview_layout_definitions * definitions);
DBOOL                       afs_toc_file_preview_page_get_sections_on_frame(afs_toc_file_preview_page * toc_file_preview_page, unsigned int * sections, unsigned int frame, const afs_toc_preview_layout_definitions * definitions);

DBOOL                       afs_toc_file_preview_page_save_file(afs_toc_file_preview_page * toc_file_preview_page, const char * file_name, DBOOL compact);
char *                      afs_toc_file_preview_page_save_string(afs_toc_file_preview_page * toc_file_preview_page, DBOOL compact);
DBOOL                       afs_toc_file_preview_page_save_xml(afs_toc_file_preview_page * toc_file_preview_page, struct mxml_node_s* out);

DBOOL                       afs_toc_file_preview_page_load_file(afs_toc_file_preview_page * toc_file_preview_page, const char * file_name);
DBOOL                       afs_toc_file_preview_page_load_string(afs_toc_file_preview_page * toc_file_preview_page, const char * in);
DBOOL                       afs_toc_file_preview_page_load_xml(afs_toc_file_preview_page * toc_file_preview_page, struct mxml_node_s* node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_FILE_PREVIEW_PAGE_H
