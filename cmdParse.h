/*********************************************************************************************************
** 文件: cmdParse.h
** 作者: scilogyhunter
** 站点: https://blog.csdn.net/scilogyhunter/category_9735056.html
** 描述: 通用命令行参数处理工具
*********************************************************************************************************/
#ifndef CMD_PARSE_H_
#define CMD_PARSE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define OPT_LABEL_CHAR              ('-')                               /*  选项标注字符                */
#define INFO_CMD_STRING             ("!")                               /*  info命令标注字符串          */
/*********************************************************************************************************
  错误号宏定义
*********************************************************************************************************/
#define NONE                        (0)
#define ERROR                       (-1)
/*********************************************************************************************************
  选项属性宏定义
  0 ~3   ATT_TYPE
  4      ATT_FUNC
  5      ATT_FORCE
  6      保留
  7      保留
  8 ~15  ATT_BUF_SIZE
  16~23  ATT_ARRAY
  24~31  保留
*********************************************************************************************************/
#define ATT_TYPE_VOID               (0)                                 /*  空类型，不需要解析          */
#define ATT_TYPE_EXIST              (1)                                 /*  选项存在置1，否则置0        */
#define ATT_TYPE_BOOL               (2)                                 /*  BOOL                        */
#define ATT_TYPE_CHAR               (3)                                 /*  CHAR,bit7应为0              */
#define ATT_TYPE_U8                 (4)                                 /*  U8                          */
#define ATT_TYPE_S8                 (5)                                 /*  S8                          */
#define ATT_TYPE_U16                (6)                                 /*  U16                         */
#define ATT_TYPE_S16                (7)                                 /*  S16                         */
#define ATT_TYPE_U32                (8)                                 /*  U32                         */
#define ATT_TYPE_S32                (9)                                 /*  S32                         */
#define ATT_TYPE_F32                (10)                                /*  F32                         */
#define ATT_TYPE_U64                (11)                                /*  U64                         */
#define ATT_TYPE_S64                (12)                                /*  S64                         */
#define ATT_TYPE_F64                (13)                                /*  F64                         */
#define ATT_TYPE_SPTR               (14)                                /*  SPTR,字符串指针             */
#define ATT_TYPE_SBUF               (15)                                /*  SBUF,字符串缓存，需拷贝     */

#define ATT_FUNC                    (1 << 4)                            /*  目标为回调函数，否则为数据  */
#define ATT_FORCE                   (1 << 5)                            /*  强制调用目标                */
#define ATT_TYPE(n)                 (((n) & 0x0f) <<  0)                /*  设置目标数据类型            */
#define ATT_BUF_SIZE(n)             (((n) & 0xff) <<  8)                /*  设置目标BUF大小sizeof(sbuf) */
#define ATT_ARRAY(n)                (((n) & 0xff) << 16)                /*  设置目标数组个数            */

#define ATT_GET_TYPE(opt)           ((opt->att & 0x0000000f) >>  0)     /*  获取目标数据类型            */
#define ATT_GET_BUF_SIZE(opt)       ((opt->att & 0x0000ff00) >>  8)     /*  获取目标BUF大小sizeof(sbuf) */
#define ATT_GET_ARRAY(opt)          ((opt->att & 0x00ff0000) >> 16)     /*  获取目标数组个数            */
/*********************************************************************************************************
  功能宏
*********************************************************************************************************/
#define GET_ARRAY_COUNT(a)          (sizeof(a) / sizeof(a[0]))          /*  获取数组成员个数            */
#define GET_OPTC(optv)              (GET_ARRAY_COUNT(optv))             /*  选项数组中选项个数          */
/*********************************************************************************************************
  选项参数结构
*********************************************************************************************************/
typedef struct {
    uint32_t        att;                                                /*  选项属性                    */
    void           *target;                                             /*  目标，类型由att决定         */
    const char     *name;                                               /*  这是选项名，前面没有短横线  */
    const char     *brief;                                              /*  选项说明                    */
    void           *targetArg;                                          /*  目标参数                    */
    uint32_t        index;                                              /*  选项在命令行参数列表中的索引*/
    uint32_t        argc;                                               /*  选项参数个数，不包含选项本身*/
} option_st;
/*********************************************************************************************************
  命令参数结构
*********************************************************************************************************/
typedef struct {
    int             argc;                                               /* command line argument count  */
    char          **argv;                                               /* command line argument value  */
    int             optc;                                               /* command line option count    */
    option_st      *optv;                                               /* command line option value    */
    void           *priv;                                               /* user private data            */
} command_st;
/*********************************************************************************************************
  选项回调函数类型定义
  arg        命令参数
  opti       选项列表中的索引号
*********************************************************************************************************/
typedef  int  (*optionCb_fp)(command_st  *cmd, int opti);
/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
int  cmdParse(command_st  *cmd);

#endif                                                                  /*  CMD_PARSE_H_                */
/*********************************************************************************************************
  END
*********************************************************************************************************/
