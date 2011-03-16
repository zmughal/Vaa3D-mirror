/* soapStub.h
   Generated by gSOAP 2.8.1 from ../webservice/v3dwebserver.h
   Copyright(C) 2000-2010, Robert van Engelen, Genivia Inc. All Rights Reserved.
   The generated code is released under one of the following licenses:
   GPL OR Genivia's license for commercial use.
*/

#ifndef soapStub_H
#define soapStub_H
#include "stdsoap2.h"

/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE_ns__LOAD_MSG
#define SOAP_TYPE_ns__LOAD_MSG (7)
/* ns:LOAD-MSG */
class SOAP_CMAC ns__LOAD_MSG
{
public:
	std::string image_path;	/* required element of type xsd:string */
	int x;	/* required element of type xsd:int */
	int y;	/* required element of type xsd:int */
	int z;	/* required element of type xsd:int */
	int c;	/* required element of type xsd:int */
	int t;	/* required element of type xsd:int */
	float intensity;	/* optional element of type xsd:float */
	int dt;	/* required element of type xsd:int */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 7; } /* = unique id SOAP_TYPE_ns__LOAD_MSG */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns__LOAD_MSG(): soap(NULL) { ns__LOAD_MSG::soap_default(NULL); }
	virtual ~ns__LOAD_MSG() { }
};
#endif

#ifndef SOAP_TYPE_ns__LOAD_RES
#define SOAP_TYPE_ns__LOAD_RES (11)
/* ns:LOAD-RES */
class SOAP_CMAC ns__LOAD_RES
{
public:
	float loadtime;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:float */
	long memorysize;	/* optional element of type xsd:long */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 11; } /* = unique id SOAP_TYPE_ns__LOAD_RES */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns__LOAD_RES(): soap(NULL) { ns__LOAD_RES::soap_default(NULL); }
	virtual ~ns__LOAD_RES() { }
};
#endif

#ifndef SOAP_TYPE_ns__helloworldResponse
#define SOAP_TYPE_ns__helloworldResponse (15)
/* ns:helloworldResponse */
struct ns__helloworldResponse
{
public:
	char **response;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns__helloworld
#define SOAP_TYPE_ns__helloworld (16)
/* ns:helloworld */
struct ns__helloworld
{
public:
	char *name;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns__msghandler
#define SOAP_TYPE_ns__msghandler (20)
/* ns:msghandler */
struct ns__msghandler
{
public:
	ns__LOAD_MSG *lm;	/* optional element of type ns:LOAD-MSG */
};
#endif

#ifndef SOAP_TYPE_ns__v3dopenfileResponse
#define SOAP_TYPE_ns__v3dopenfileResponse (22)
/* ns:v3dopenfileResponse */
struct ns__v3dopenfileResponse
{
public:
	char **v3dfn;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns__v3dopenfile
#define SOAP_TYPE_ns__v3dopenfile (23)
/* ns:v3dopenfile */
struct ns__v3dopenfile
{
public:
	char *fn;	/* optional element of type xsd:string */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (24)
/* SOAP Header: */
struct SOAP_ENV__Header
{
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (25)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (27)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (30)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (31)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
public:
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
};
#endif

#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


#endif

/* End of soapStub.h */
