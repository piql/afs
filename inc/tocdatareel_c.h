#ifndef AFS_TOC_DATA_REEL_H
#define AFS_TOC_DATA_REEL_H

/*****************************************************************************
**
**  Definition of the toc data reel interface
**
**  Creation date:  2018/05/05
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2018 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "tocfiles.h"
#include "frameranges.h"
#include "gvector.h"
#include "mxml.h"

enum afs_toc_data_reel_section
{
    AFS_TOC_DATA_REEL_SECTION_DIGITAL = 0x1,
    AFS_TOC_DATA_REEL_SECTION_PREVIEW = 0x2,
    AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS = 0x4,
    AFS_TOC_DATA_REEL_SECTION_ALL = AFS_TOC_DATA_REEL_SECTION_DIGITAL | AFS_TOC_DATA_REEL_SECTION_PREVIEW | AFS_TOC_DATA_REEL_SECTION_FILE_FORMAT_DESCRIPTIONS
};

// Definition structure afs_data_reel
typedef struct afs_toc_data_reel_s
{
    char *          id;
    afs_toc_files * files;
} afs_toc_data_reel;


afs_toc_data_reel * afs_toc_data_reel_create();
afs_toc_data_reel * afs_toc_data_reel_create2(const char * id);
void                afs_toc_data_reel_free(afs_toc_data_reel * toc_data_reel);

afs_toc_data_reel * afs_toc_data_reel_clone(const afs_toc_data_reel * toc_data_reel);
DBOOL               afs_toc_data_reel_equal(const afs_toc_data_reel * toc_data_reel1, const afs_toc_data_reel * toc_data_reel2);

void                afs_toc_data_reel_set_id(afs_toc_data_reel * toc_data_reel, const char * id);
DBOOL               afs_toc_data_reel_add_file(afs_toc_data_reel * toc_data_reel, afs_toc_file * toc_file);
afs_toc_file *      afs_toc_data_reel_get_file_by_index(const afs_toc_data_reel * toc_data_reel, const unsigned int index);
afs_toc_file *      afs_toc_data_reel_get_file_by_id(const afs_toc_data_reel * toc_data_reel, const int id);
afs_toc_file *      afs_toc_data_reel_get_file_by_unique_id(const afs_toc_data_reel * toc_data_reel, const char * unique_id);

afs_toc_files *     afs_toc_data_reel_get_parent_files(const afs_toc_data_reel * toc_data_reel, const int types);
afs_toc_files *     afs_toc_data_reel_get_child_files(const afs_toc_data_reel * toc_data_reel, const int types);
afs_toc_files *     afs_toc_data_reel_get_standalone_files(const afs_toc_data_reel * toc_data_reel, const int types);

unsigned int        afs_toc_data_reel_file_count(const afs_toc_data_reel * toc_data_reel);
unsigned int        afs_toc_data_reel_frame_count(const afs_toc_data_reel * toc_data_reel, const unsigned int section);
int                 afs_toc_data_reel_first_frame(const afs_toc_data_reel * toc_data_reel, const unsigned int section);
int                 afs_toc_data_reel_last_frame(const afs_toc_data_reel * toc_data_reel, const unsigned int section);
void                afs_toc_data_reel_add_frame_offset(afs_toc_data_reel * toc_data_reel, const unsigned int offset, const unsigned int section);
boxing_int64        afs_toc_data_reel_size_bytes(const afs_toc_data_reel * toc_data_reel);
DBOOL               afs_toc_data_reel_is_valid(const afs_toc_data_reel * toc_data_reel);

afs_frame_ranges *  afs_toc_data_reel_get_frame_ranges(const afs_toc_files * toc_files);

DBOOL               afs_toc_data_reel_save_file(afs_toc_data_reel * toc_data_reel, const char * file_name, DBOOL compact);
char *              afs_toc_data_reel_save_string(afs_toc_data_reel * toc_data_reel, DBOOL compact);
DBOOL               afs_toc_data_reel_save_xml(afs_toc_data_reel * toc_data_reel, mxml_node_t* out);

DBOOL               afs_toc_data_reel_load_file(afs_toc_data_reel * toc_data_reel, const char * file_name);
DBOOL               afs_toc_data_reel_load_string(afs_toc_data_reel * toc_data_reel, const char * in);
DBOOL               afs_toc_data_reel_load_xml(afs_toc_data_reel * toc_data_reel, mxml_node_t* node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_DATA_REEL_H
