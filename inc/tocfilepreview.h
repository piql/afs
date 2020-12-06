#ifndef AFS_TOC_FILE_PREVIEW_H
#define AFS_TOC_FILE_PREVIEW_H

/*****************************************************************************
**
**  Definition of the toc file preview interface
**
**  Creation date:  2017/04/20
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "gvector.h"
#include "tocfilepreviewpage.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef gvector afs_toc_file_preview_pages;

// Definition structure afs_toc_file_preview
typedef struct afs_toc_file_preview_s
{
    afs_toc_file_preview_pages* pages;
    int                         dpi;
} afs_toc_file_preview;


afs_toc_file_preview* afs_toc_file_preview_create();
afs_toc_file_preview* afs_toc_file_preview_create2(afs_toc_file_preview_pages* pages);

void  afs_toc_file_preview_init(afs_toc_file_preview * toc_file_preview);
void  afs_toc_file_preview_init2(afs_toc_file_preview * toc_file_preview, afs_toc_file_preview_pages* pages);

void  afs_toc_file_preview_free(afs_toc_file_preview * toc_file_preview);

afs_toc_file_preview* afs_toc_file_preview_clone(const afs_toc_file_preview * toc_file_preview);
DBOOL                 afs_toc_file_preview_equal(afs_toc_file_preview * toc_file_preview1, afs_toc_file_preview * toc_file_preview2);

DBOOL                        afs_toc_file_preview_add_page(afs_toc_file_preview * toc_file_preview, afs_toc_file_preview_page * toc_file_preview_page);
afs_toc_file_preview_page *  afs_toc_file_preview_get_page(afs_toc_file_preview * toc_file_preview, unsigned int index);
unsigned int                 afs_toc_file_preview_get_page_count(afs_toc_file_preview * toc_file_preview);
int                          afs_toc_file_preview_get_start_frame(afs_toc_file_preview * toc_file_preview);
void                         afs_toc_file_preview_add_frame_offset(afs_toc_file_preview * toc_file_preview, unsigned int offset);
DBOOL                        afs_toc_file_preview_is_valid(afs_toc_file_preview * toc_file_preview);
void                         afs_toc_file_preview_set_dpi(afs_toc_file_preview * toc_file_preview, int dpi);
DBOOL                        afs_toc_file_preview_get_frames_count(afs_toc_file_preview * toc_file_preview, unsigned int * frames_count, const afs_toc_preview_layout_definitions * definitions);

DBOOL         afs_toc_file_preview_save_file(afs_toc_file_preview * toc_file_preview, const char * file_name, DBOOL compact);
char *        afs_toc_file_preview_save_string(afs_toc_file_preview * toc_file_preview, DBOOL compact);
DBOOL         afs_toc_file_preview_save_xml(afs_toc_file_preview * toc_file_preview, struct mxml_node_s* out);

DBOOL         afs_toc_file_preview_load_file(afs_toc_file_preview * toc_file_preview, const char * file_name);
DBOOL         afs_toc_file_preview_load_string(afs_toc_file_preview * toc_file_preview, const char * in);
DBOOL         afs_toc_file_preview_load_xml(afs_toc_file_preview * toc_file_preview, struct mxml_node_s* node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_FILE_PREVIEW_H
