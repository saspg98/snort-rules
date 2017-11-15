/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>

!!! XXX DOES NOT USE BUILT-IN DETECTION FUNCTIONS XXX !!!

alert udp $EXTERNAL_NET 53 -> $HOME_NET any (msg:"BAD-TRAFFIC dns root nameserver poisoning attempt"; flow:to_client; content:"|00 01|"; depth:2; offset:4; byte_test:2,>,1,8; content:"|00 02 00 01|"; distance:0; metadata:policy security-ips drop, policy balanced-ips drop; reference:cve,2008-1454; reference:url, technet.microsoft.com/en-us/security/bulletin/MS08-037; classtype:misc-attack; sid:13887; rev:7;)
 */

#include <string.h>

#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

//#define DEBUG
#ifdef DEBUG
#define DEBUG_WRAP(code) code
#else
#define DEBUG_WRAP(code)
#endif

#define ROOT_NAME_BUF_LEN 256

/* declare detection functions */
int rule13887eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flow:established, to_client; */
static FlowFlags rule13887flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule13887option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule13887flow0
    }
};
// content:"|00 01|", offset 4, depth 2; 
static ContentInfo rule13887content1 = 
{
    (uint8_t *) "|00 01|", /* pattern (now in snort content format) */
    2, /* depth */
    4, /* offset */
    CONTENT_BUF_NORMALIZED, /* flags */ 
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule13887option1 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule13887content1
    }
};
/* byte_test:size 2, value 1, operator >, offset 8; */
static ByteData rule13887byte_test2 = 
{
    2, /* size */
    CHECK_GT, /* operator */
    1, /* value */
    8, /* offset */
    0, /*multiplier */
    BYTE_BIG_ENDIAN|CONTENT_BUF_NORMALIZED|EXTRACT_AS_BYTE /* flags */
};

static RuleOption rule13887option2 = 
{
    OPTION_TYPE_BYTE_TEST,
    {
        &rule13887byte_test2
    }
};

#ifndef CONTENT_FAST_PATTERN
#define CONTENT_FAST_PATTERN 0
#endif
// content:"|00 02 00 01|", depth 0, relative, fast_pattern; 
static ContentInfo rule13887content3 = 
{
    (uint8_t *) "|00 02 00 01|", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_RELATIVE|CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule13887option3 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule13887content3
    }
};

/* references for sid 13887 */
/* reference: cve "2008-1454"; */
static RuleReference rule13887ref1 = 
{
    "cve", /* type */
    "2008-1454" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS08-037"; */
static RuleReference rule13887ref2 = 
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS08-037" /* value */
};

static RuleReference *rule13887refs[] =
{
    &rule13887ref1,
    &rule13887ref2,
    NULL
};

/* metadata for sid 13887 */
/* metadata:policy balanced-ips drop, policy security-ips drop; */
//static RuleMetaData rule13887policy1 = 
//{
//    "policy balanced-ips drop"
//};
//
//static RuleMetaData rule13887policy2 = 
//{
//    "policy security-ips drop"
//};


static RuleMetaData *rule13887metadata[] =
{
//    &rule13887policy1,
//    &rule13887policy2,
    NULL
};

RuleOption *rule13887options[] =
{
    &rule13887option0,
    &rule13887option1,
    &rule13887option2,
    &rule13887option3,
    NULL
};

Rule rule13887 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_UDP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "53", /* SRCPORT   */
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid (HARDCODED!!!) */
       13887, /* sigid */
       12, /* revision */
   
       "misc-attack", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "PROTOCOL-DNS dns root nameserver poisoning attempt",     /* message */
       rule13887refs /* ptr to references */
       ,rule13887metadata
   },
   rule13887options, /* ptr to rule options */
   &rule13887eval, /* DOES NOT use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule13887eval(void *p) {
   const uint8_t *cursor_normal = 0, *beg_of_payload, *end_of_payload;
   SFSnortPacket *sp = (SFSnortPacket *) p;

   uint16_t num_of_queries, num_of_answers, num_of_authorities;

   uint16_t comp_offset;
   uint16_t data_len;
   uint8_t seglen;
   uint8_t new_root_name[ROOT_NAME_BUF_LEN], prev_root_name[ROOT_NAME_BUF_LEN];
   const uint8_t *rr_ptr;
   uint8_t root_name_len = 0;

   // flags
   uint8_t first_NS_record;

   // cruft
   uint16_t i,j;
   DEBUG_WRAP(uint16_t k);

   DEBUG_WRAP(printf("rule13887eval dns auth nameserver enter\n"));

   if(sp == NULL)
      return RULE_NOMATCH;

   if(sp->payload == NULL)
      return RULE_NOMATCH;
    
   // flow:established, to_client;
   if(checkFlow(p, rule13887options[0]->option_u.flowFlags) <= 0 )
      return RULE_NOMATCH;

   DEBUG_WRAP(printf("passed flow\n"));

   // content:"|00 01|", offset 4, depth 2;
   if(contentMatch(p, rule13887options[1]->option_u.content, &cursor_normal) <= 0)
      return RULE_NOMATCH;

   DEBUG_WRAP(printf("passed content\n"));

   // byte_test:size 2, value 1, operator >, offset 8;
   if(byteTest(p, rule13887options[2]->option_u.byte, cursor_normal) <= 0)
      return RULE_NOMATCH;

   DEBUG_WRAP(printf("passed byte test\n"));

// This check doesn't have to actually be here; the structure is only for the fast pattern matcher
//   // content:"|00 02 00 01|", depth 0, relative, fast_pattern;
//   if (contentMatch(p, rule13887options[3]->option_u.content, &cursor_normal) <= 0) 
//       return RULE_MATCH;
//
//   DEBUG_WRAP(printf("passed content\n"));   

   if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
      return RULE_NOMATCH;

   if(end_of_payload - beg_of_payload < 25)
      return RULE_NOMATCH;

   cursor_normal = beg_of_payload + 4;

   num_of_queries = *cursor_normal++ << 8;
   num_of_queries |= *cursor_normal++;

   num_of_answers = *cursor_normal++ << 8;
   num_of_answers |= *cursor_normal++;

   // Our byte_test assures us num_of_authorities is >= 2 so we have
   // something to compare later
   num_of_authorities = *cursor_normal++ << 8;
   num_of_authorities |= *cursor_normal++;

   cursor_normal += 2; // skip over count of additional RRs

   DEBUG_WRAP(printf("num_of_queries=%d num_of_answers=%d num_of_authorities=%d\n", num_of_queries, num_of_answers, num_of_authorities));
 
   // Jump over the query records
   for(i = 0; i < num_of_queries; i++) {
      DEBUG_WRAP(printf("processing query %d, payload_offset=%x\n", i, cursor_normal - beg_of_payload));

      while(cursor_normal < end_of_payload && *cursor_normal != 0 && *cursor_normal < 0xc0) // !((*cursor_normal & 0xc0) == 0xc0)
         cursor_normal += *cursor_normal + 1; 

      if(cursor_normal >= end_of_payload)
         return RULE_NOMATCH;

      // (two bytes for pointer (2 bytes)  or null byte (1 byte)) + type (2 bytes) + class (2 bytes)
      cursor_normal += (*cursor_normal >= 0xc0) ? 6 : 5;  // (*cursor_normal & 0xc0) == 0xc0
   }

   //if(cursor_normal >= end_of_payload)
   //   return RULE_NOMATCH;

   // Jump over the answers
   for(i = 0; i < num_of_answers; i++) {
      DEBUG_WRAP(printf("processing answer %d, payload_offset=%x\n", i, cursor_normal - beg_of_payload));

      while(cursor_normal < end_of_payload && *cursor_normal != 0 && *cursor_normal < 0xc0) { // performance improvement, !((*cursor_normal & 0xc0) == 0xc0)  
         cursor_normal += *cursor_normal + 1; 
      }

      if(cursor_normal + 4 > end_of_payload)
         return RULE_NOMATCH;

      // (two bytes for pointer (2 bytes) or a single null byte (1 byte)) + type (2 bytes) + class (2 bytes) + ttl (4 bytes)
      cursor_normal += (*cursor_normal >= 0xc0) ? 2 : 1;
 
      if(*cursor_normal++ == 0x00 && *cursor_normal++ == 0x05) {
         DEBUG_WRAP(printf("We found a CNAME record; let's just bail rather than figure this out for real.\n"));
         return RULE_NOMATCH;
      }
      cursor_normal += 6;

      if(cursor_normal + 2 > end_of_payload)
         return RULE_NOMATCH;

      data_len = (*cursor_normal++) << 8;
      data_len |= *cursor_normal++;

      cursor_normal += data_len;
   } 
      
   // Now on to the meat... The Authoritative nameservers.  If we
   // change roots, alert.
   first_NS_record = 1;
   for(i = 0; i < num_of_authorities; i++) {
      DEBUG_WRAP(printf("processing authority %d, payload_offset=%x\n", i, cursor_normal - beg_of_payload));
      if(cursor_normal + 12 >= end_of_payload)
         return RULE_NOMATCH;

      j=0; // index into expanded root name
      while(cursor_normal < end_of_payload && *cursor_normal != 0 && *cursor_normal < 0xc0) { // performance improvement, !((*cursor_normal & 0xc0) == 0xc0)
         // copy the size and data for each segment
         seglen = *cursor_normal++;
         new_root_name[j++] = seglen;
         DEBUG_WRAP(printf("seglen = %d\n", seglen));
         if((j + seglen < ROOT_NAME_BUF_LEN) && (cursor_normal + seglen < end_of_payload)) {
            DEBUG_WRAP(printf("copying bytes\n"));
            memcpy(&(new_root_name[j]), cursor_normal, seglen);
            cursor_normal += seglen;
            j += seglen;
         } else {
            return RULE_NOMATCH;
         }
      }
   
      // Either we're at a pointer, a null, or out of data
      if((cursor_normal + 1 < end_of_payload) && *cursor_normal >= 0xc0) { // pointer // performance improvement, (*cursor_normal & 0xc0) == 0xc0
         // Set the pointer
         comp_offset = (*cursor_normal++ & 0x3F) << 8;
         comp_offset |= *cursor_normal++;
         rr_ptr = beg_of_payload + comp_offset;
   
         DEBUG_WRAP(printf("offset = %d\n", comp_offset));
   
         while(rr_ptr < end_of_payload && *rr_ptr != 0 && *rr_ptr < 0xc0) { // performance improvement, !((*rr_ptr & 0xc0) == 0xc0)
            // copy the size and data for each segment
            seglen = *rr_ptr++;
            new_root_name[j++] = seglen;
            DEBUG_WRAP(printf("seglen = %d\n", seglen));
            if((j + seglen < ROOT_NAME_BUF_LEN) && (rr_ptr + seglen < end_of_payload)) {
               DEBUG_WRAP(printf("copying bytes\n"));
               memcpy(&(new_root_name[j]), rr_ptr, seglen);
               rr_ptr += seglen;
               j += seglen;
            } else {
               return RULE_NOMATCH;
            }
         }
   
         DEBUG_WRAP(printf("root: "));
         DEBUG_WRAP(for(k=0; k < j; k++) printf("%c", new_root_name[k]));
         DEBUG_WRAP(printf("\n"));
   
         // we only handle one level of compression
         if(rr_ptr >= end_of_payload || *rr_ptr >= 0xc0) { // performance improvement, ((*rr_ptr & 0xc0) == 0xc0)
            DEBUG_WRAP(printf("Multiple compression - Stopping\n"));
            return RULE_NOMATCH;
         }
      } else if((cursor_normal < end_of_payload) && (*cursor_normal == 0)) { // null (end of name string)
         cursor_normal++;
      } else { // out of data
         DEBUG_WRAP(printf("Out of data\n"));
         return RULE_NOMATCH;
      }

      if(cursor_normal + 2 > end_of_payload)
         return RULE_NOMATCH;

      // Only now, after traversing the name, can we determine if it's an
      // NS record and that we didn't waste our time storing the data first.
      DEBUG_WRAP(printf("Record Type 0x%02x%02x\n", *cursor_normal, *(cursor_normal+1)));

      if((*cursor_normal == 0) && (*(cursor_normal+1) == 2)) {
         DEBUG_WRAP(printf("Found NS record (0x00002)\n"));
         if(first_NS_record) {
            // Store the initial root
            DEBUG_WRAP(printf("First NS Record.  Storing %d bytes\n", j));
            memcpy(prev_root_name, new_root_name, j);
            root_name_len = j;
            first_NS_record = 0;
         } else {
            // Compare the new root to previous root.  If it changes, alert.
            DEBUG_WRAP(printf("Comparing NS Record to stored name.  j=%d root_name_len=%d\n", j, root_name_len));
            DEBUG_WRAP(for(k=0;k<j;k++) printf("%02x %02x\n", prev_root_name[k], new_root_name[k]); printf("\n"));
            if((j != root_name_len) || memcmp(prev_root_name, new_root_name, j))
               return RULE_MATCH;
         }
      } 

      if(cursor_normal + 10 > end_of_payload)
         return RULE_NOMATCH;

      // Process the rest of the record so we can get to the next record
      cursor_normal += 2+2+4; // type/class/ttl
      data_len = *cursor_normal++ << 8; // len of nameserver string
      data_len |= *cursor_normal++;
      DEBUG_WRAP(printf("data_len=%d\n", data_len));
      cursor_normal += data_len;
   }

   return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule13887,
    NULL
};
*/
