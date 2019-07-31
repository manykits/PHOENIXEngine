//
//  player_head.h
//  DVRunning
//
//  Created by DFung on 16/3/9.
//  Copyright © 2016年 DevinVon. All rights reserved.
//

#ifndef player_head_h
#define player_head_h

#include <stdio.h>
#include <stdlib.h>

struct frm_head
{
    uint32_t    seq;
    uint32_t    frm_sz;
    uint32_t    offset;
    uint32_t    date;
    uint8_t     type;
    
}__attribute__((packed));


struct rtp_head
{
    uint32_t    seq;
    uint32_t    frm_sz;
    uint32_t    offset;
    uint8_t     type;
    
}__attribute__((packed));


struct player_data
{
    int              size;
    int              type;
    struct frm_head  head;
    struct rtp_head  rtp_head;
    unsigned char    *data;
};

struct player_data1
{
    uint8_t          type;
    uint32_t         ts;
    uint32_t         seq;
    char             file[128];
    uint32_t         size;
    uint8_t          data[0];
};

/**
 *  Release memory that player_data occupied.
 */
void free_player_data(struct player_data * p_data);


#endif /* player_head_h */
