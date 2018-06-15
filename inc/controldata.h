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

#ifdef __cplusplus
extern "C" {
#endif

#include    "administrativemetadata.h"
#include    "technicalmetadata.h"
#include    "boxing/bool.h"
#include    "mxml.h"

// Definition structure afs_control_data
typedef struct afs_control_data_s
{
    afs_administrative_metadata * administrative_metadata;
    afs_technical_metadata *      technical_metadata;
} afs_control_data;

afs_control_data*  afs_control_data_create();
void               afs_control_data_init(afs_control_data*);
void               afs_control_data_init2(afs_control_data*, afs_administrative_metadata*, afs_technical_metadata*);
void               afs_control_data_free(afs_control_data*);

DBOOL              afs_control_data_save_xml(mxml_node_t * out, afs_control_data* control_data);
DBOOL              afs_control_data_save_xml_file(const char * file_name, afs_control_data* control_data, DBOOL compact);
DBOOL              afs_control_data_load_xml(afs_control_data* control_data, mxml_node_t * in);
DBOOL              afs_control_data_load_xml_file(afs_control_data* control_data, const char * file_name);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // AFS_CONTROL_DATA_H
