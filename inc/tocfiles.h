#ifndef AFS_TOC_FILES_H
#define AFS_TOC_FILES_H

/*****************************************************************************
**
**  Definition of the TOC files interface
**
**  Creation date:  2017/05/26
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "tocfile.h"
#include "toc/previewlayoutdefinitions.h"
#include "gvector.h"
#include "mxml.h"

#ifdef __cplusplus
extern "C" {
#endif

// Definition structure afs_toc_files
typedef gvector afs_toc_indices;

typedef struct afs_toc_files_s
{
    afs_toc_indices * tocs;

    int reference_count;
} afs_toc_files;

afs_toc_files * afs_toc_files_create();
afs_toc_files * afs_toc_files_create2(afs_toc_indices * tocs);

void            afs_toc_files_init(afs_toc_files * toc_files);
void            afs_toc_files_init2(afs_toc_files * toc_files, afs_toc_indices * tocs);

void            afs_toc_files_free(afs_toc_files * toc_files);

afs_toc_files*  afs_toc_files_clone(afs_toc_files * toc_files);
afs_toc_files * afs_toc_files_get_new_reference(afs_toc_files * toc_files);
DBOOL           afs_toc_files_equal(afs_toc_files * toc_files1, afs_toc_files * toc_files2);

DBOOL           afs_toc_files_add_toc(afs_toc_files * toc_files, afs_toc_file * toc_file);
afs_toc_file *  afs_toc_files_get_toc(const afs_toc_files * toc_files, const size_t index);
size_t          afs_toc_files_get_tocs_count(const afs_toc_files * toc_files);

int             afs_toc_files_get_duration_frames(afs_toc_files * toc_files, unsigned int type, afs_toc_preview_layout_definitions * definitions);
int             afs_toc_files_get_first_frame(afs_toc_files * toc_files, unsigned int type);
int             afs_toc_files_get_last_frame(afs_toc_files * toc_files, unsigned int type, afs_toc_preview_layout_definitions * definitions);
void            afs_toc_files_add_frame_offset(afs_toc_files * toc_files, unsigned int offset, unsigned int type);
DBOOL           afs_toc_files_is_valid(afs_toc_files * toc_files);

DBOOL           afs_toc_files_save_file(afs_toc_files * toc_files, const char * file_name, DBOOL compact);
char *          afs_toc_files_save_string(afs_toc_files * toc_files, DBOOL compact);
DBOOL           afs_toc_files_save_xml(afs_toc_files * toc_files, mxml_node_t* out);

DBOOL           afs_toc_files_load_file(afs_toc_files * toc_files, const char * file_name);
DBOOL           afs_toc_files_load_string(afs_toc_files * toc_files, const char * in);
DBOOL           afs_toc_files_load_xml(afs_toc_files * toc_files, mxml_node_t* node);

char *          afs_toc_files_save_as_table(afs_toc_files * toc_files);
char *          afs_toc_files_save_as_location_table(afs_toc_files * toc_files);
char *          afs_toc_files_save_as_metadata_table(afs_toc_files * toc_files);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_FILES_H
