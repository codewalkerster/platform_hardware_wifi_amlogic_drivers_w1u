/**
****************************************************************************************
*
* @file aml_mdns_offload.c
*
* Copyright (C) Amlogic, Inc. All rights reserved (2022-2023).
*
* @brief android mDNS offload
*
****************************************************************************************
*/
#define AML_MODULE  MDNS

#include "aml_mdns_offload.h"
#include "wifi_drv_main.h"
#include "fi_sdio.h"

/// The maximum number of response data that can be added
#define MDNS_INDEX_ERR (-1)
#define MDNS_INDEX_MAX (3)
#define MDNS_OFFLOAD_FEATURE 1

extern int mdns_index;
extern int mdns_misscnt;
extern int mdns_hitcnt;
extern int mdns_passthrough_state;

static u32_boolean setOffloadState(u32_boolean enabled)
{
    uint32_t ret;
    struct drv_private *drv_priv = drv_get_drv_priv();
    printk(" MDNS_OFFLOAD_FEATURE STATE: %d\n", enabled);

#ifdef MDNS_OFFLOAD_FEATURE
    drv_set_mdns_offload_state(drv_priv, enabled);
    if (enabled != 0)
    {
        printk(" MDNS_OFFLOAD_FEATURE is enabled!\n");
        ret = true;
        goto exit;
    }
    ret = false;
#else
    printk(" MDNS_OFFLOAD_FEATURE is disabled!\n");
    //aml_mdns_set_offload_state(0);
    drv_set_mdns_offload_state(drv_priv, 0);
    ret = false;
#endif

exit:
    printk(" enabled:%d,ret:%d\n", enabled, ret);
    return ret;
}

static void resetAll()
{
    struct drv_private *drv_priv = drv_get_drv_priv();
    drv_set_mdns_reset_all(drv_priv);
}

static int addProtocolResponses(mdnsProtocolData *offloadData)
{
    matchCriteria list_lmac[MDNS_LIST_CRITERIA_MAX] = {0};
    struct drv_private *drv_priv = drv_get_drv_priv();
    int i = 0;
    //int ret;
    int index = MDNS_INDEX_ERR;

    // change type to reduce fw mem
    for (i = 0; (i < offloadData->matchCriteriaListNum) && (i < MDNS_LIST_CRITERIA_MAX); ++i) {
        list_lmac[i].nameOffset = offloadData->matchCriteriaList[i].nameOffset;
        list_lmac[i].type = offloadData->matchCriteriaList[i].type;
    }

    if (offloadData->rawOffloadPacketLen <= MDNS_RAW_DATA_LENGTH_MAX)
    {
        drv_set_mdns_add_protocol_data_status(drv_priv);

        index = drv_set_mdns_add_protocol_data(drv_priv, list_lmac, offloadData);
    }
    else
    {
        printk(" mdns frame size err\n");
    }

    return index;
}

static void removeProtocolResponses(int recordKey)
{
    struct drv_private *drv_priv = drv_get_drv_priv();
    drv_set_mdns_remove_protocol_data(drv_priv, recordKey);
}

static int getAndResetHitCounter(int recordKey)
{
    struct drv_private *drv_priv = drv_get_drv_priv();
    drv_set_mdns_get_reset_hit_counter(drv_priv, recordKey);
    return mdns_hitcnt;
}

static int getAndResetMissCounter()
{
    struct drv_private *drv_priv = drv_get_drv_priv();
    drv_set_mdns_get_reset_miss_counter(drv_priv);
    return mdns_misscnt;
}

static u32_boolean addToPassthroughList(char *qname)
{
    struct drv_private *drv_priv = drv_get_drv_priv();
    drv_set_mdns_add_passthrough_list(drv_priv, qname, strlen(qname));
    return mdns_passthrough_state;
}

static void removeFromPassthroughList(char *qname)
{
    struct drv_private *drv_priv = drv_get_drv_priv();
    drv_set_mdns_remove_passthrough_list(drv_priv, qname, strlen(qname));
}

static void setPassthroughBehavior(
    passthroughBehavior behavior)
{
    struct drv_private *drv_priv = drv_get_drv_priv();
    drv_set_passthrough_behavior(drv_priv, behavior);
}

const struct s_mdns_offload_ops mdns_offload_ops = {
    .setOffloadState = setOffloadState,
#ifdef MDNS_OFFLOAD_FEATURE
    .resetAll = resetAll,
    .addProtocolResponses = addProtocolResponses,
    .removeProtocolResponses = removeProtocolResponses,
    .getAndResetHitCounter = getAndResetHitCounter,
    .getAndResetMissCounter = getAndResetMissCounter,
    .addToPassthroughList = addToPassthroughList,
    .removeFromPassthroughList = removeFromPassthroughList,
    .setPassthroughBehavior = setPassthroughBehavior,
#else
    .setOffloadState = NULL,
    .resetAll = NULL,
    .addProtocolResponses = NULL,
    .removeProtocolResponses = NULL,
    .getAndResetHitCounter = NULL,
    .getAndResetMissCounter = NULL,
    .addToPassthroughList = NULL,
    .removeFromPassthroughList = NULL,
    .setPassthroughBehavior = NULL,
#endif
};

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 3, 0))
const struct nla_policy mdns_offload_attr_policy[WIFI_MDNS_OFFLOAD_ATTRIBUTE_MAX] = {
    [WIFI_MDNS_OFFLOAD_ATTRIBUTE_STATE]                = { .type = NLA_U32, .len = sizeof(uint32_t) },
    [WIFI_MDNS_OFFLOAD_ATTRIBUTE_NETWORK_INTERFACE]    = { .type = NLA_NUL_STRING },
    [WIFI_MDNS_OFFLOAD_ATTRIBUTE_OFFLOAD_PKT_LEN]      = { .type = NLA_U32, .len = sizeof(uint32_t) },
    [WIFI_MDNS_OFFLOAD_ATTRIBUTE_OFFLOAD_PKT_DATA]     = { .type = NLA_BINARY },
    [WIFI_MDNS_OFFLOAD_ATTRIBUTE_MATCH_CRITERIA_NUM]   = { .type = NLA_U32, .len = sizeof(uint32_t) },
    [WIFI_MDNS_OFFLOAD_ATTRIBUTE_MATCH_CRITERIA_DATA]  = { .type = NLA_BINARY },
    [WIFI_MDNS_OFFLOAD_ATTRIBUTE_RECORD_KEY]           = { .type = NLA_U32, .len = sizeof(uint32_t) },
    [WIFI_MDNS_OFFLOAD_ATTRIBUTE_QNAME]                = { .type = NLA_NUL_STRING },
    [WIFI_MDNS_OFFLOAD_ATTRIBUTE_PASSTHROUGH_BEHAVIOR] = { .type = NLA_U32, .len = sizeof(uint32_t) },
};
#endif
