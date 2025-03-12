#ifndef AFS_CONTROL_DATA_H
#define AFS_CONTROL_DATA_H

/*****************************************************************************
**
**  Definition of the Control Data interface
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

#include    "administrativemetadata.h"
#include    "technicalmetadata.h"
#include    "boxing/bool.h"
#include    "mxml.h"

#ifdef __cplusplus
extern "C" {
#endif

// Definition structure afs_control_data
typedef struct afs_control_data_s
{
    afs_administrative_metadata * administrative_metadata;
    afs_technical_metadata *      technical_metadata;

    int reference_count;
} afs_control_data;

afs_control_data * afs_control_data_create();
afs_control_data * afs_control_data_create2(afs_administrative_metadata * administrative_metadata, afs_technical_metadata * technical_metadata);
void               afs_control_data_free(afs_control_data * control_data);

afs_control_data * afs_control_data_clone(const afs_control_data * control_data);
afs_control_data * afs_control_data_get_new_reference(afs_control_data * control_data);
DBOOL              afs_control_data_equal(const afs_control_data * control_data1, const afs_control_data * control_data2);

DBOOL              afs_control_data_save_file(afs_control_data * control_data, const char * file_name, DBOOL compact);
char *             afs_control_data_save_string(afs_control_data * control_data, DBOOL compact);
DBOOL              afs_control_data_save_xml(afs_control_data * control_data, mxml_node_t * out);

DBOOL              afs_control_data_load_file(afs_control_data * control_data, const char * file_name);
DBOOL              afs_control_data_load_string(afs_control_data * control_data, const char * in);
DBOOL              afs_control_data_load_xml(afs_control_data * control_data, mxml_node_t * in);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_CONTROL_DATA_H
