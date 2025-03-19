#ifndef AFS_ADMINISTRATIVE_METADATA_H
#define AFS_ADMINISTRATIVE_METADATA_H

/*****************************************************************************
**
**  Definition of the Administrative Metadata interface
**
**  Creation date:  2016/12/28
**  Created by:     Piql AS
**
**
**  Copyright (c) 2016 Piql AS. All rights reserved.
**
**  This file is part of the afs library
**
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include    "boxing/bool.h"

struct mxml_node_s;

// Definition structure afs_administrative_metadata
typedef struct afs_administrative_metadata_s
{
    char * reel_id;
    char * print_reel_id;
    char * title;
    char * description;
    char * creator;
    char * creation_date;

    int reference_count;
} afs_administrative_metadata;


afs_administrative_metadata * afs_administrative_metadata_create();
afs_administrative_metadata * afs_administrative_metadata_create2(
                                  const char * reel_id,
                                  const char * print_reel_id,
                                  const char * title,
                                  const char * description,
                                  const char * creator,
                                  const char * creation_date);
void                          afs_administrative_metadata_free(afs_administrative_metadata * administrative_metadata);

afs_administrative_metadata * afs_administrative_metadata_clone(const afs_administrative_metadata * administrative_metadata);
afs_administrative_metadata * afs_administrative_metadata_get_new_reference(afs_administrative_metadata * administrative_metadata);
DBOOL                         afs_administrative_metadata_equal(const afs_administrative_metadata * administrative_metadata1, const afs_administrative_metadata * administrative_metadata2);

void                          afs_administrative_metadata_set_reel_id(afs_administrative_metadata * administrative_metadata, const char * reel_id);
void                          afs_administrative_metadata_set_print_reel_id(afs_administrative_metadata * administrative_metadata, const char * print_reel_id);
void                          afs_administrative_metadata_set_title(afs_administrative_metadata * administrative_metadata, const char * title);
void                          afs_administrative_metadata_set_description(afs_administrative_metadata * administrative_metadata, const char * description);
void                          afs_administrative_metadata_set_creator(afs_administrative_metadata * administrative_metadata, const char * creator);
void                          afs_administrative_metadata_set_creation_date(afs_administrative_metadata * administrative_metadata, const char * creation_date);

DBOOL                         afs_administrative_metadata_save_file(afs_administrative_metadata * administrative_metadata, const char * file_name, DBOOL compact);
char *                        afs_administrative_metadata_save_string(afs_administrative_metadata * administrative_metadata, DBOOL compact);
DBOOL                         afs_administrative_metadata_save_xml(afs_administrative_metadata * administrative_metadata, struct mxml_node_s * out);

DBOOL                         afs_administrative_metadata_load_file(afs_administrative_metadata * administrative_metadata, const char * file_name);
DBOOL                         afs_administrative_metadata_load_string(afs_administrative_metadata * administrative_metadata, const char * in);
DBOOL                         afs_administrative_metadata_load_xml(afs_administrative_metadata * administrative_metadata, struct mxml_node_s * in);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_ADMINISTRATIVE_METADATA_H
