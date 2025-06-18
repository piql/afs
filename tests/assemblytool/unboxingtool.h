#ifndef UNBOXING_TOOL_H
#define UNBOXING_TOOL_H

/*****************************************************************************
**
**  Definition of the Unboxing Tool interface
**
**  Creation date:  2017/05/10
**  Created by:     Aleksandr Ivanov
**
**
**  Copyright (c) 2017 Piql AS. All rights reserved.
**
**  This file is part of the boxing library
**
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "unboxingutility.h"
#include "controldata.h"
#include "sha1.h"
#include "boxing/config.h"

typedef struct unboxing_parameters_s
{
    const char    *input_files;
    boxing_config *format;
    DBOOL         is_raw;
    DBOOL         is_read;
    size_t        x_size;
    size_t        y_size;

    gvector *    unbox_files_numbers;

    DBOOL        show_help_information;
} unboxing_parameters;


int  boxing_unbox_data_frame_file(afs_toc_file * current_toc, unboxing_parameters parameters);
int  boxing_unbox_control_frame_file(unboxing_parameters parameters);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // UNBOXING_TOOL_H
