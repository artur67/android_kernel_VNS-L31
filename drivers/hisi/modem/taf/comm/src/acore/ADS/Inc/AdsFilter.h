

#ifndef __ADSFILTER_H__
#define __ADSFILTER_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "vos.h"
#include "AdsDeviceInterface.h"
#include "hi_list.h"
#include "AdsTcpIpTypeDef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define ADS_FILTER_MAX_LIST_NUM                 (256)
#define ADS_FILTER_DEFAULT_AGING_TIMELEN        (60)
#define ADS_FILTER_SECOND_TRANSFER_JIFFIES      (1000)

#define ADS_FILTER_IPV4_HDR_LEN                 (20)            /* IPv4 首部长度 */
#define ADS_FILTER_ICMP_HDR_LEN                 (8)             /* ICMP 首部长度 */

#define ADS_MAX_IPV4_ADDR_LEN                   (16)            /* IPv4地址最大字符串长度 */

#define ADS_IPPROTO_ICMP                        (0x01)          /* ICMP protocol */
#define ADS_IPPROTO_IGMP                        (0x02)          /* IGMP protocol */
#define ADS_IPPROTO_TCP                         (0x06)          /* TCP protocol */
#define ADS_IPPROTO_UDP                         (0x11)          /* UDP protocol */

#define ADS_IP_CE                               (0x8000)        /* Flag: "Congestion" */
#define ADS_IP_DF                               (0x4000)        /* Flag: "Don't Fragment" */
#define ADS_IP_MF                               (0x2000)        /* Flag: "More Fragments" */
#define ADS_IP_OFFSET                           (0x1FFF)        /* "Fragment Offset" part */

/* ICMP报文的type字段宏定义 */
#define ADS_ICMP_ECHOREPLY                      (0)             /* Echo Reply           */
#define ADS_ICMP_DEST_UNREACH                   (3)             /* Destination Unreachable  */
#define ADS_ICMP_SOURCE_QUENCH                  (4)             /* Source Quench        */
#define ADS_ICMP_REDIRECT                       (5)             /* Redirect (change route)  */
#define ADS_ICMP_ECHOREQUEST                    (8)             /* Echo Request         */
#define ADS_ICMP_TIME_EXCEEDED                  (11)            /* Time Exceeded        */
#define ADS_ICMP_PARAMETERPROB                  (12)            /* Parameter Problem    */
#define ADS_ICMP_TIMESTAMP                      (13)            /* Timestamp Request    */
#define ADS_ICMP_TIMESTAMPREPLY                 (14)            /* Timestamp Reply      */
#define ADS_ICMP_INFO_REQUEST                   (15)            /* Information Request  */
#define ADS_ICMP_INFO_REPLY                     (16)            /* Information Reply    */
#define ADS_ICMP_ADDRESS                        (17)            /* Address Mask Request */
#define ADS_ICMP_ADDRESSREPLY                   (18)            /* Address Mask Reply   */

/* 数组元素个数计算所使用的宏 */
#define ADS_FILTER_ARRAY_SIZE(a)                (sizeof((a)) / sizeof((a[0])))

#define ADS_FILTER_GET_IPV6_ADDR()              (&g_stAdsFilterCtx.unIPv6Addr)
#define ADS_FILTER_GET_LIST(index)              (&(g_stAdsFilterCtx.astLists[index]))
#define ADS_FILTER_GET_AGING_TIME_LEN()         (g_stAdsFilterCtx.ulAgingTimeLen)
#define ADS_FILTER_SET_AGING_TIME_LEN(len)      (g_stAdsFilterCtx.ulAgingTimeLen = (len))

#define ADS_FILTER_GET_DL_ICMP_FUNC_TBL_ADDR(ucType)     (&(g_astAdsFilterDecodeDlIcmpFuncTbl[ucType]))
#define ADS_FILTER_GET_DL_ICMP_FUNC_TBL_SIZE()           (ADS_FILTER_ARRAY_SIZE(g_astAdsFilterDecodeDlIcmpFuncTbl))

/* 可维可测信息统计所用宏 */
#define ADS_FILTER_DBG_GET_STATS_ARRAY_ADDR()            (&g_aulAdsFilterStats[0])
#define ADS_FILTER_DBG_GET_STATS_BY_INDEX(enPktType)     (g_aulAdsFilterStats[enPktType])
#define ADS_FILTER_DBG_STATISTIC(enPktType, n)           (g_aulAdsFilterStats[enPktType] += (n))

#define ADS_FILTER_MALLOC(size)                 ADS_FILTER_HeapAlloc(size)
#define ADS_FILTER_FREE(addr)                   ADS_FILTER_HeapFree(addr)

/* 检查是否达到老化时间 */
#define ADS_FILTER_IS_AGED(time)\
    ((0 != ADS_FILTER_GET_AGING_TIME_LEN()) \
   &&(time_after_eq(jiffies, (ADS_FILTER_GET_AGING_TIME_LEN() + (time)))))

/* 检查IP首部里的相关信息是否匹配 ADS_FILTER_IPV4_HEADER_STRU */
#define ADS_FILTER_IS_IPHDR_MATCH(ipheader1, ipheader2)\
    ( ((ipheader1)->ulSrcAddr == (ipheader2)->ulSrcAddr) \
   && ((ipheader1)->ulDstAddr == (ipheader2)->ulDstAddr) \
   && ((ipheader1)->ucProtocol == (ipheader2)->ucProtocol) )

/* 检查TCP包的过滤条件是否匹配 ADS_FILTER_IPV4_TCP_STRU */
#define ADS_FILTER_IS_TCP_PKT_MATCH(tcppkt1, tcppkt2)\
    ( ((tcppkt1)->usSrcPort == (tcppkt2)->usSrcPort) \
   && ((tcppkt1)->usDstPort == (tcppkt2)->usDstPort) )

/* 检查UDP包的过滤条件是否匹配 ADS_FILTER_IPV4_UDP_STRU */
#define ADS_FILTER_IS_UDP_PKT_MATCH(udppkt1, udppkt2)\
    ( ((udppkt1)->usSrcPort == (udppkt2)->usSrcPort) \
   && ((udppkt1)->usDstPort == (udppkt2)->usDstPort) )

/* 检查ICMP包的过滤条件是否匹配 ADS_FILTER_IPV4_ICMP_STRU */
#define ADS_FILTER_IS_ICMP_PKT_MATCH(icmppkt1, icmppkt2)\
    ( ((icmppkt1)->usIdentifier == (icmppkt2)->usIdentifier) \
   && ((icmppkt1)->usSeqNum == (icmppkt2)->usSeqNum) )

/* 检查分片包的过滤条件是否匹配 ADS_FILTER_IPV4_FRAGMENT_STRU */
#define ADS_FILTER_IS_FRAGMENT_MATCH(fragmentpkt1, fragmentpkt2)\
        ( (fragmentpkt1)->usIdentification == (fragmentpkt2)->usIdentification)

/* 检查IPV6地址是否相同 ADS_IPV6_ADDR_UN */
#define ADS_FILTER_IS_IPV6_ADDR_IDENTICAL(punIpv6Addr1, punIpv6Addr2) \
    (0 == ( ((punIpv6Addr1)->aulIpAddr[0] ^ (punIpv6Addr2)->aulIpAddr[0]) | \
            ((punIpv6Addr1)->aulIpAddr[1] ^ (punIpv6Addr2)->aulIpAddr[1]) | \
            ((punIpv6Addr1)->aulIpAddr[2] ^ (punIpv6Addr2)->aulIpAddr[2]) | \
            ((punIpv6Addr1)->aulIpAddr[3] ^ (punIpv6Addr2)->aulIpAddr[3]) ) )


/* 过滤表索引号映射关系:
    对于TCP\UCP包，过滤表索引值为源端口号低8位,
    对于ICMP包，过滤表索引值为Sequence Num低8位,
    对于下行分片包(非首片)使用IP Identification低8位作为索引 */
#define ADS_FILTER_GET_INDEX(pfilter) \
    ((VOS_UINT8)(VOS_NTOHS(*((VOS_UINT16 *)&((pfilter)->unFilter))) & 0xFF))


/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/*****************************************************************************
枚举名    : ADS_FILTER_PKT_TYPE_ENUM
枚举说明  : ADS过滤数据包类型

  1.日    期   : 2013年6月3日
    作    者   : L47619
    修改内容   : V3R3 Share-PDP项目修改
*****************************************************************************/
enum ADS_FILTER_PKT_TYPE_ENUM
{
    ADS_FILTER_PKT_TYPE_TCP,                                /* 数据包类型为TCP */
    ADS_FILTER_PKT_TYPE_UDP,                                /* 数据包类型为UDP */
    ADS_FILTER_PKT_TYPE_ICMP,                               /* 数据包类型为ICMP */
    ADS_FILTER_PKT_TYPE_FRAGMENT,                           /* 数据包类型为分片包(非首片) */

    ADS_FILTER_PKT_TYPE_BUTT
};
typedef VOS_UINT32 ADS_FILTER_PKT_TYPE_ENUM_UINT32;

/*****************************************************************************
枚举名    : ADS_FILTER_ORIG_PKT_ENUM
枚举说明  : ADS数据包原始报文类型

  1.日    期   : 2013年6月14日
    作    者   : L47619
    修改内容   : V3R3 Share-PDP项目修改
*****************************************************************************/
enum ADS_FILTER_ORIG_PKT_ENUM
{
    ADS_FILTER_ORIG_PKT_UL_IPV4_TCP,                                   /* 原始数据包类型为上行IPv4 TCP */
    ADS_FILTER_ORIG_PKT_UL_IPV4_UDP,                                   /* 原始数据包类型为上行IPv4 UDP */
    ADS_FILTER_ORIG_PKT_UL_IPV4_ECHOREQ,                               /* 原始数据包类型为上行IPv4 ECHO REQ */
    ADS_FILTER_ORIG_PKT_UL_IPV4_NOT_FIRST_FRAGMENT,                    /* 原始数据包类型为上行IPv4 分片包(非首片) */
    ADS_FILTER_ORIG_PKT_UL_IPV4_NOT_SUPPORT,                           /* 原始数据包类型为上行不支持的IPv4报文 */
    ADS_FILTER_ORIG_PKT_UL_IPV6_PKT,                                   /* 原始数据包类型为上行IPv6报文 */

    ADS_FILTER_ORIG_PKT_DL_IPV4_TCP,                                   /* 原始数据包类型为下行IPv4 TCP */
    ADS_FILTER_ORIG_PKT_DL_IPV4_UDP,                                   /* 原始数据包类型为下行IPv4 UDP */
    ADS_FILTER_ORIG_PKT_DL_IPV4_ECHOREPLY,                             /* 原始数据包类型为下行IPv4 ECHO REPLY */
    ADS_FILTER_ORIG_PKT_DL_IPV4_ICMPERROR,                             /* 原始数据包类型为下行IPv4 ICMP差错报文 */
    ADS_FILTER_ORIG_PKT_DL_IPV4_FIRST_FRAGMENT,                        /* 原始数据包类型为下行IPv4 分片包(首片) */
    ADS_FILTER_ORIG_PKT_DL_IPV4_NOT_FIRST_FRAGMENT,                    /* 原始数据包类型为下行IPv4 分片包(非首片) */
    ADS_FILTER_ORIG_PKT_DL_IPV6_PKT,                                   /* 原始数据包类型为下行IPv6包 */

    ADS_FILTER_ORIG_PKT_BUTT
};
typedef VOS_UINT32 ADS_FILTER_ORIG_PKT_ENUM_UINT32;

/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/

/*****************************************************************************
  7 UNION定义
*****************************************************************************/

/*****************************************************************************
  8 STRUCT定义
*****************************************************************************/

/*****************************************************************************
 结构名    : ADS_FILTER_IPV4_HEADER_STRU
 结构说明  : IPV4的IP首部信息中用于过滤匹配的条件
 1.日    期   : 2013年06月03日
   作    者   : l00198894
   修改内容   : V3R3 Share-PDP项目创建
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulSrcAddr;                              /* source address */
    VOS_UINT32                          ulDstAddr;                              /* dest address */
    VOS_UINT8                           ucProtocol;                             /* protocol */
    VOS_UINT8                           aucReserved[3];                         /* reserved */
} ADS_FILTER_IPV4_HEADER_STRU;

/*****************************************************************************
 结构名    : ADS_FILTER_IPV4_TCP_STRU
 结构说明  : IPV4的TCP报文过滤匹配条件
 1.日    期   : 2013年06月03日
   作    者   : L47619
   修改内容   : V3R3 Share-PDP项目创建
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usSrcPort;                              /* source port */
    VOS_UINT16                          usDstPort;                              /* dest port */
} ADS_FILTER_IPV4_TCP_STRU;

/*****************************************************************************
 结构名    : ADS_FILTER_IPV4_UDP_STRU
 结构说明  : IPV4的UDP报文过滤匹配条件
 1.日    期   : 2013年06月03日
   作    者   : L47619
   修改内容   : V3R3 Share-PDP项目创建
*****************************************************************************/
typedef ADS_FILTER_IPV4_TCP_STRU    ADS_FILTER_IPV4_UDP_STRU;

/*****************************************************************************
 结构名    : ADS_FILTER_IPV4_ICMP_STRU
 结构说明  : IPV4的ICMP报文过滤匹配条件
 1.日    期   : 2013年06月03日
   作    者   : L47619
   修改内容   : V3R3 Share-PDP项目创建
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usSeqNum;                               /* ICMP首部中的Sequence number */
    VOS_UINT16                          usIdentifier;                           /* ICMP首部中的Identifier */
} ADS_FILTER_IPV4_ICMP_STRU;

/*****************************************************************************
 结构名    : ADS_FILTER_IPV4_FRAGMENT_STRU
 结构说明  : IPV4的下行分片报文，记录首片的源地址、目的地址和IP首部中的
             Identification作为过滤匹配条件
 1.日    期   : 2013年06月03日
   作    者   : L47619
   修改内容   : V3R3 Share-PDP项目创建
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usIdentification;                       /* IP首部中的identification */
    VOS_UINT8                           aucReserved[2];                         /* reserved */
} ADS_FILTER_IPV4_FRAGMENT_STRU;

/*****************************************************************************
 结构名    : ADS_FILTER_IPV4_INFO_STRU
 结构说明  : IPV4的过滤匹配条件
 1.日    期   : 2013年06月03日
   作    者   : L47619
   修改内容   : V3R3 Share-PDP项目创建
*****************************************************************************/
typedef struct
{
    ADS_FILTER_PKT_TYPE_ENUM_UINT32     enPktType;
    VOS_UINT8                           aucReserved[4];
    unsigned long                       ulTmrCnt;
    ADS_FILTER_IPV4_HEADER_STRU         stIPHeader;

    union
    {
        ADS_FILTER_IPV4_TCP_STRU        stTcpFilter;
        ADS_FILTER_IPV4_UDP_STRU        stUdpFilter;
        ADS_FILTER_IPV4_ICMP_STRU       stIcmpFilter;
        ADS_FILTER_IPV4_FRAGMENT_STRU   stFragmentFilter;
    }unFilter;

}ADS_FILTER_IPV4_INFO_STRU;


/*****************************************************************************
 结构名    : ADS_FILTER_DECODE_DL_ICMP_FUNC
 结构说明  : 下行ICMP包解析时所用结构体，包括对应ICMP type解析的函数指针及
              对应原始报文类型
 1.日    期   : 2013年06月18日
   作    者   : l00198894
   修改内容   : V3R3 Share-PDP项目创建
*****************************************************************************/
typedef VOS_UINT32 (*ADS_FILTER_DECODE_DL_ICMP_FUNC)(
    ADS_IPV4_HDR_STRU                  *pstIPv4Hdr,
    ADS_FILTER_IPV4_INFO_STRU          *pstIPv4Filter);

typedef struct
{
    ADS_FILTER_DECODE_DL_ICMP_FUNC      pFunc;
    ADS_FILTER_ORIG_PKT_ENUM_UINT32     enOrigPktType;
    VOS_UINT8                           aucReserved[4];
}ADS_FILTER_DECODE_DL_ICMP_FUNC_STRU;


/*****************************************************************************
 结构名    : ADS_FILTER_NODE_STRU
 结构说明  : 下行过滤链表节点结构体
 1.日    期   : 2013年06月03日
   作    者   : l00198894
   修改内容   : V3R3 Share-PDP项目创建
*****************************************************************************/
typedef struct
{
    ADS_FILTER_IPV4_INFO_STRU           stFilter;
    HI_LIST_S                           stList;
}ADS_FILTER_NODE_STRU;

/*****************************************************************************
 结构名    : ADS_FILTER_CTX_STRU
 结构说明  : ADS过滤功能相关上下文
 1.日    期   : 2013年06月03日
   作    者   : l00198894
   修改内容   : V3R3 Share-PDP项目创建
*****************************************************************************/
typedef struct
{
    HI_LIST_S                           astLists[ADS_FILTER_MAX_LIST_NUM];      /* ADS下行数据过滤表 */
    ADS_IPV6_ADDR_UN                    unIPv6Addr;                             /* ADS用于过滤下行IPv6类型数据 */
    VOS_UINT32                          ulAgingTimeLen;                         /* ADS过滤节点老化时长, 单位毫秒 */
    VOS_UINT8                           aucReserved[4];
}ADS_FILTER_CTX_STRU;


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/

/*****************************************************************************
  10 函数声明
*****************************************************************************/

VOS_VOID ADS_FILTER_ResetIPv6Addr(VOS_VOID);

VOS_VOID ADS_FILTER_InitListsHead(VOS_VOID);

VOS_VOID ADS_FILTER_InitCtx(VOS_VOID);

VOS_VOID* ADS_FILTER_HeapAlloc(VOS_UINT32 ulSize);

VOS_VOID ADS_FILTER_HeapFree(VOS_VOID *pAddr);

VOS_VOID ADS_FILTER_AddFilter(
    ADS_FILTER_IPV4_INFO_STRU          *pstFilter);

VOS_VOID ADS_FILTER_ResetLists(VOS_VOID);

VOS_VOID ADS_FILTER_Reset(VOS_VOID);

VOS_UINT32 ADS_FILTER_IsInfoMatch(
    ADS_FILTER_IPV4_INFO_STRU          *pstFilter1,
    ADS_FILTER_IPV4_INFO_STRU          *pstFilter2);

VOS_UINT32 ADS_FILTER_Match(
    ADS_FILTER_IPV4_INFO_STRU          *pstFilter);

VOS_VOID ADS_FILTER_SaveIPAddrInfo(
    ADS_FILTER_IP_ADDR_INFO_STRU       *pstFilterIpAddr);

VOS_UINT32 ADS_FILTER_DecodeUlPacket(
    IMM_ZC_STRU                        *pstData,
    ADS_FILTER_IPV4_INFO_STRU          *pstIPv4Filter);

VOS_VOID ADS_FILTER_ProcUlPacket(
    IMM_ZC_STRU                        *pstData,
    ADS_PKT_TYPE_ENUM_UINT8             enIpType);

VOS_VOID ADS_FILTER_DecodeDlIPv4NotFirstFragPacket(
    ADS_IPV4_HDR_STRU                  *pstIPv4Hdr,
    ADS_FILTER_IPV4_INFO_STRU          *pstIPv4Filter);

VOS_UINT32 ADS_FILTER_DecodeDlIPv4EchoReplyPacket(
    ADS_IPV4_HDR_STRU                  *pstIPv4Hdr,
    ADS_FILTER_IPV4_INFO_STRU          *pstIPv4Filter);

VOS_UINT32 ADS_FILTER_DecodeDlIPv4IcmpErrorPacket(
    ADS_IPV4_HDR_STRU                  *pstIPv4Hdr,
    ADS_FILTER_IPV4_INFO_STRU          *pstIPv4Filter);

VOS_VOID ADS_FILTER_DecodeDlIPv4TcpPacket(
    ADS_IPV4_HDR_STRU                  *pstIPv4Hdr,
    ADS_FILTER_IPV4_INFO_STRU          *pstIPv4Filter);

VOS_VOID ADS_FILTER_DecodeDlIPv4UdpPacket(
    ADS_IPV4_HDR_STRU                  *pstIPv4Hdr,
    ADS_FILTER_IPV4_INFO_STRU          *pstIPv4Filter);

VOS_UINT32 ADS_FILTER_DecodeDlIPv4FragPacket(
    ADS_IPV4_HDR_STRU                  *pstIPv4Hdr,
    ADS_FILTER_IPV4_INFO_STRU          *pstIPv4Filter,
    ADS_FILTER_ORIG_PKT_ENUM_UINT32    *penOrigPktType);

VOS_UINT32 ADS_FILTER_DecodeDlIPv4Packet(
    ADS_IPV4_HDR_STRU                  *pstIPv4Hdr,
    ADS_FILTER_IPV4_INFO_STRU          *pstIPv4Filter,
    ADS_FILTER_ORIG_PKT_ENUM_UINT32    *penOrigPktType);

VOS_UINT32 ADS_FILTER_ProcDlIPv4Packet(
    IMM_ZC_STRU                        *pstData);

VOS_UINT32 ADS_FILTER_ProcDlIPv6Packet(
    IMM_ZC_STRU                        *pstData);

VOS_UINT32 ADS_FILTER_ProcDlPacket(
    IMM_ZC_STRU                        *pstData,
    ADS_PKT_TYPE_ENUM_UINT8             enIpType);


VOS_VOID ADS_FILTER_ShowStatisticInfo(VOS_VOID);

VOS_VOID ADS_FILTER_ResetStatisticInfo(VOS_VOID);


VOS_VOID ADS_FILTER_ShowIPv6Addr(VOS_VOID);




#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of AdsFilter.h */
