#ifndef AFS_TOC_METADATA_SOURCE_H
#define AFS_TOC_METADATA_SOURCE_H

/*****************************************************************************
**
**  Definition of the toc metadata source interface
**
**  Creation date:  2017/08/22
**  Created by:     Oleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "boxing/config.h"
#include "gvector.h"
#include "mxml.h"


/*!
 *  \ingroup metadata
 *
 *  Undefined ID states.
 */

enum
{
    FILE_ID_UNDEFINED = -1,
    ID_UNDEFINED = -1
};

typedef gvector afs_toc_metadata_sources;
typedef gvector afs_toc_metadata_source_tags;

// Definition structure afs_toc_file
typedef struct afs_toc_metadata_source_s
{
    char *                         format;
    int                            id;
    int                            file_id;
    char *                         xml_data;
    afs_toc_metadata_source_tags * tags;
} afs_toc_metadata_source;

afs_toc_metadata_source * afs_toc_metadata_source_create();
afs_toc_metadata_source * afs_toc_metadata_source_create2(const char * format, int file_id);
afs_toc_metadata_source * afs_toc_metadata_source_create3(const char * format, int file_id, int id);
afs_toc_metadata_source * afs_toc_metadata_source_create4(const char * format, int file_id, const char * xml_data);
afs_toc_metadata_source * afs_toc_metadata_source_create5(const char * format, int file_id, const char * xml_data, int id);
afs_toc_metadata_source * afs_toc_metadata_source_create6(const char * format, int file_id, const char * xml_data, const char * tags);
afs_toc_metadata_source * afs_toc_metadata_source_create7(const char * format, int file_id, const char * xml_data, const char * tags, int id);
afs_toc_metadata_source * afs_toc_metadata_source_create8(const char * format, int file_id, afs_toc_metadata_source_tags * tags);
afs_toc_metadata_source * afs_toc_metadata_source_create9(const char * format, int file_id, afs_toc_metadata_source_tags * tags, int id);

void afs_toc_metadata_source_init(afs_toc_metadata_source * toc_metadata_source);
void afs_toc_metadata_source_init2(afs_toc_metadata_source * toc_metadata_source, const char * format, int file_id, const char * xml_data, const char * tags, int id);
void afs_toc_metadata_source_init3(afs_toc_metadata_source * toc_metadata_source, const char * format, int file_id, afs_toc_metadata_source_tags * tags, int id);

void afs_toc_metadata_source_free(afs_toc_metadata_source * toc_metadata_source);

afs_toc_metadata_source * afs_toc_metadata_source_clone(afs_toc_metadata_source * toc_metadata_source);
DBOOL                     afs_toc_metadata_source_equal(afs_toc_metadata_source * toc_metadata_source1, afs_toc_metadata_source * toc_metadata_source2);
DBOOL                     afs_toc_metadata_source_is_valid(afs_toc_metadata_source * toc_metadata_source);
void                      afs_toc_metadata_source_set_tags(afs_toc_metadata_source * toc_metadata_source, const char * tags);
char *                    afs_toc_metadata_source_get_tags(afs_toc_metadata_source * toc_metadata_source);

DBOOL                     afs_toc_metadata_source_save_file(afs_toc_metadata_source * toc_metadata_source, const char * file_name, DBOOL compact);
char *                    afs_toc_metadata_source_save_string(afs_toc_metadata_source * toc_metadata_source, DBOOL compact);
DBOOL                     afs_toc_metadata_source_save_xml(afs_toc_metadata_source * toc_metadata_source, mxml_node_t* out);

DBOOL                     afs_toc_metadata_source_load_file(afs_toc_metadata_source * toc_metadata_source, const char * file_name);
DBOOL                     afs_toc_metadata_source_load_string(afs_toc_metadata_source * toc_metadata_source, const char * in);
DBOOL                     afs_toc_metadata_source_load_xml(afs_toc_metadata_source * toc_metadata_source, mxml_node_t* input_node);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_TOC_METADATA_SOURCE_H
