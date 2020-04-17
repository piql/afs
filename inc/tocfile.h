#ifndef AFS_TOC_FILE_H
#define AFS_TOC_FILE_H

/*****************************************************************************
**
**  Definition of the toc data file interface
**
**  Creation date:  2017/02/22
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#include "tocfilepreview.h"
#include "tocdatafilemetadata_c.h"
#include "boxing/config.h"
#include "gvector.h"
#include "mxml.h"

#ifdef __cplusplus
extern "C" {
#endif

enum afs_toc_file_type
{
    AFS_TOC_FILE_TYPE_UNDEFINED = 0x0,
    AFS_TOC_FILE_TYPE_DIGITAL = 0x1,
    AFS_TOC_FILE_TYPE_PREVIEW = 0x2,
    AFS_TOC_FILE_TYPE_ALL = AFS_TOC_FILE_TYPE_DIGITAL | AFS_TOC_FILE_TYPE_PREVIEW
};


/*!
 *  \ingroup file
 *
 *  Parent ID types.
 */

enum
{
    AFS_TOC_FILE_PARENT = -1,
    AFS_TOC_FILE_NO_PARENT = -2,
    AFS_TOC_FILE_ID_UNDEFINED = -1
};

// Definition structure afs_toc_file
typedef struct afs_toc_file_s
{
    char *                       name;
    char *                       date;
    char *                       checksum;
    boxing_int64                 size;
    int                          id;
    int                          start_frame;
    int                          start_byte;
    int                          end_frame;
    int                          end_byte;
    char *                       unique_id;
    int                          parent_id;
    int                          types;
    afs_toc_file_preview *       preview;
    char *                       file_format;
    afs_toc_data_file_metadata * metadata;
} afs_toc_file;


afs_toc_file * afs_toc_file_create();
afs_toc_file * afs_toc_file_create2(
    const char *                 name,
    const char *                 date,
    const char *                 checksum,
    boxing_int64                 size,
    int                          id,
    int                          start_frame,
    int                          start_byte,
    int                          end_frame,
    int                          end_byte,
    const char *                 unique_id,
    int                          parent_id,
    int                          types,
    afs_toc_file_preview *       preview,
    const char *                 file_format,
    afs_toc_data_file_metadata * metadata);

void                afs_toc_file_init(afs_toc_file *);
void                afs_toc_file_init2(
    afs_toc_file *               toc_data_file,
    const char *                 name,
    const char *                 date,
    const char *                 checksum,
    boxing_int64                 size,
    int                          id,
    int                          start_frame,
    int                          start_byte,
    int                          end_frame,
    int                          end_byte,
    const char *                 unique_id,
    int                          parent_id,
    int                          types,
    afs_toc_file_preview *       preview,
    const char *                 file_format,
    afs_toc_data_file_metadata * metadata);

void                afs_toc_file_free(afs_toc_file *);

afs_toc_file *      afs_toc_file_clone(afs_toc_file *);
DBOOL               afs_toc_file_equal(afs_toc_file *, afs_toc_file *);
DBOOL               afs_toc_file_not_equal(afs_toc_file *, afs_toc_file *);

void                afs_toc_file_add_frame_offset(afs_toc_file *, unsigned int);
void                afs_toc_file_add_type(afs_toc_file *, unsigned int);

void                afs_toc_file_set_name(afs_toc_file *, const char *);
void                afs_toc_file_set_date(afs_toc_file *, const char *);
void                afs_toc_file_set_checksum(afs_toc_file *, const char *);
void                afs_toc_file_set_unique_id(afs_toc_file *, const char *);
void                afs_toc_file_set_file_format(afs_toc_file *, const char *);
void                afs_toc_file_set_preview(afs_toc_file *, const afs_toc_file_preview *);
void                afs_toc_file_set_metadata(afs_toc_file *, const afs_toc_data_file_metadata *);

DBOOL               afs_toc_file_is_parent(const afs_toc_file *);
DBOOL               afs_toc_file_has_parent(afs_toc_file *);
DBOOL               afs_toc_file_is_standalone(afs_toc_file *);
DBOOL               afs_toc_file_is_digital(const afs_toc_file *);
DBOOL               afs_toc_file_is_preview(const afs_toc_file *);
DBOOL               afs_toc_file_has_metadata(const afs_toc_file *);
DBOOL               afs_toc_file_is_valid(const afs_toc_file *);
int                 afs_toc_file_get_duration_frames(afs_toc_file *);
boxing_int64        afs_toc_file_size(afs_toc_file *);

DBOOL               afs_toc_file_load_file(afs_toc_file * toc_data_file, const char * file_name);
DBOOL               afs_toc_file_load_string(afs_toc_file * toc_data_file, const char * in);
DBOOL               afs_toc_file_load_xml(afs_toc_file * toc_data_file, mxml_node_t* node);
DBOOL               afs_toc_file_load_data_v1(afs_toc_file * toc_data_file, mxml_node_t* node);
DBOOL               afs_toc_file_load_data_v2(afs_toc_file * toc_data_file, mxml_node_t* node);

DBOOL               afs_toc_file_save_file(afs_toc_file * toc_data_file, const char * file_name, DBOOL compact, DBOOL data_v1);
char *              afs_toc_file_save_string(afs_toc_file * toc_data_file, DBOOL compact, DBOOL data_v1);
DBOOL               afs_toc_file_save_xml(afs_toc_file * toc_data_file, mxml_node_t* out, DBOOL data_v1);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_FILE_H
