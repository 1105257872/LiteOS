/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/* Includes -----------------------------------------------------------------*/
#include "test_object_security.h"
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "object_comm.h"

    static atiny_param_t s_atiny_params;
    static int s_malloc_failed_index = 0;

    extern void copy_security_object(lwm2m_object_t *objectDest, lwm2m_object_t *objectSrc);
    extern void display_security_object(lwm2m_object_t *object);
    extern void clean_security_object(lwm2m_object_t *objectP);
    extern lwm2m_object_t * get_security_object(uint16_t serverId,atiny_param_t* atiny_params,lwm2m_context_t* lwm2m_context);
    extern char *security_get_uri(lwm2m_object_t *objectP, uint16_t secObjInstID);

    extern lwm2m_data_t *lwm2m_data_new(int size);
    extern void *lwm2m_malloc(size_t s);


    static lwm2m_data_t *stub_lwm2m_data_new(int size)
    {
        return NULL;
    }

    static void *stub_lwm2m_malloc(size_t s)
    {
        static int i = 0;
        if( i++ == s_malloc_failed_index)
        {
            i = 0;
            return NULL;
        }
        if( s_malloc_failed_index == 0xff)
        {
            i = 0;
        }
        return atiny_malloc(s);
    }

	int stub_lwm2m_data_decode_int(const lwm2m_data_t *dataP, int64_t *valueP)
    {
        int result = 1;
		*valueP = 2;
		return result;
    }
	static int stub_lwm2m_data_decode_int_1(const lwm2m_data_t *dataP,
                          int64_t *valueP){return 1;}
	
}//extern "C"
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestObjectSecurity::TestObjectSecurity()
{
	printf("in TestObjectSecurity 84\n");
    TEST_ADD(TestObjectSecurity::test_copy_security_object);
	printf("in TestObjectSecurity 86\n");
    TEST_ADD(TestObjectSecurity::test_display_security_object);
	printf("in TestObjectSecurity 88\n");
    TEST_ADD(TestObjectSecurity::test_clean_security_object);
	printf("in TestObjectSecurity 90\n");
    TEST_ADD(TestObjectSecurity::test_get_security_object);
	printf("in TestObjectSecurity 92\n");
    TEST_ADD(TestObjectSecurity::test_security_get_uri);
	printf("in TestObjectSecurity 94\n");

    TEST_ADD(TestObjectSecurity::test_prv_security_read);
    TEST_ADD(TestObjectSecurity::test_prv_security_write);
    TEST_ADD(TestObjectSecurity::test_prv_security_create);
    TEST_ADD(TestObjectSecurity::test_prv_security_delete);

	
	printf("in TestObjectSecurity 97\n");
}
TestObjectSecurity::~TestObjectSecurity()
{
}

void TestObjectSecurity::test_copy_security_object(void)
{
    uint16_t serverId = 123;

    lwm2m_object_t *pSrcSecurityObject = NULL;
    lwm2m_object_t *pDestSecurityObject = NULL;
    lwm2m_context_t context;

    memset(&context, 0, sizeof(lwm2m_context_t));

    pSrcSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSrcSecurityObject != NULL), "get_security_object(...) failed");
        
    pDestSecurityObject = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
    TEST_ASSERT_MSG((pDestSecurityObject != NULL), "lwm2m_malloc(...) failed");
    // p == NULL is not checked in function copy_security_object(lwm2m_object_t *, lwm2m_object_t *)
    uint8_t ret = 0;

    stubInfo stub_info;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &stub_info);

    s_malloc_failed_index = 0;
    copy_security_object(pDestSecurityObject, pSrcSecurityObject);
    TEST_ASSERT_MSG((ret == 0), "copy_security_object(...) failed");

    s_malloc_failed_index = 1;
    copy_security_object(pDestSecurityObject, pSrcSecurityObject);
    TEST_ASSERT_MSG((ret == 0), "copy_security_object(...) failed");

    cleanStub(&stub_info);

    copy_security_object(pDestSecurityObject, pSrcSecurityObject);
    TEST_ASSERT_MSG((ret == 0), "copy_security_object(...) failed");

    clean_security_object(pDestSecurityObject);
    clean_security_object(pSrcSecurityObject);

    ////////////////////////////////////////////////
    pSrcSecurityObject = NULL;
    pDestSecurityObject = NULL;
    
    pSrcSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSrcSecurityObject != NULL), "get_security_object(...) failed");
        
    pDestSecurityObject = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
    TEST_ASSERT_MSG((pDestSecurityObject != NULL), "lwm2m_malloc(...) failed");

    security_instance_t *instanceSrc = (security_instance_t *)pSrcSecurityObject->instanceList;
    if(instanceSrc != NULL){
        instanceSrc->securityMode = LWM2M_SECURITY_MODE_PRE_SHARED_KEY;
        char publicIdentity[] = "abcd";
        char secretKey[] = "123456";
        uint16_t publicIdLen = sizeof(publicIdentity);
        uint16_t secretKeyLen = sizeof(secretKey);
        instanceSrc->publicIdentity = (char*)lwm2m_malloc(publicIdLen);
        instanceSrc->secretKey = (char*)lwm2m_malloc(secretKeyLen);
        memcpy(instanceSrc->publicIdentity, publicIdentity, publicIdLen);
        memcpy(instanceSrc->secretKey, secretKey, secretKeyLen);
        instanceSrc->publicIdLen = publicIdLen;
        instanceSrc->secretKeyLen = secretKeyLen;
    }
    copy_security_object(pDestSecurityObject, pSrcSecurityObject);
    TEST_ASSERT_MSG((ret == 0), "copy_security_object(...) failed");
    
    clean_security_object(pDestSecurityObject);
    clean_security_object(pSrcSecurityObject);
}
void TestObjectSecurity::test_display_security_object(void)
{
    uint16_t serverId = 123;

    lwm2m_object_t *pSecurityObject = NULL;
    lwm2m_context_t context;

    memset(&context, 0, sizeof(lwm2m_context_t));

    // p == NULL is not checked in function display_security_object(lwm2m_object_t *)

    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject != NULL), "get_security_object(...) failed");
    
    uint8_t ret = 0;
    display_security_object(pSecurityObject);
    TEST_ASSERT_MSG((ret == 0), "display_security_object(...) failed");
    clean_security_object(pSecurityObject);
}

void TestObjectSecurity::test_clean_security_object(void)
{
    uint16_t serverId = 123;

    lwm2m_object_t *pSecurityObject = NULL;
    lwm2m_context_t context;

    memset(&context, 0, sizeof(lwm2m_context_t));

    uint8_t ret = 0;
    clean_security_object(pSecurityObject);
    TEST_ASSERT_MSG((ret == 0), "clean_security_object(...) failed");

    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject != NULL), "get_security_object(...) failed");
    clean_security_object(pSecurityObject);
    TEST_ASSERT_MSG((ret == 0), "clean_security_object(...) failed");
}

void TestObjectSecurity::test_get_security_object(void)
{
    uint16_t serverId = 123;
    char *serverUri = (char *)"coap://192.168.1.106:5368";

    lwm2m_object_t *pSecurityObject = NULL;
    lwm2m_context_t context;

    memset(&context, 0, sizeof(lwm2m_context_t));

    /***************** test malloc failed *********************************/
    stubInfo stub_info;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &stub_info);

    s_malloc_failed_index = 0;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    s_atiny_params.server_params.bootstrap_mode = BOOTSTRAP_CLIENT_INITIATED;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    s_atiny_params.server_params.bootstrap_mode = BOOTSTRAP_SEQUENCE;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    //context.bs_sequence_state = BS_SEQUENCE_STATE_FACTORY;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    s_atiny_params.server_params.bootstrap_mode = (atiny_bootstrap_type_e)0xff;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    s_atiny_params.server_params.bootstrap_mode = BOOTSTRAP_FACTORY;
    s_malloc_failed_index = 1;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    s_malloc_failed_index = 2;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    s_atiny_params.server_params.bootstrap_mode = BOOTSTRAP_CLIENT_INITIATED;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    s_atiny_params.server_params.bootstrap_mode = BOOTSTRAP_SEQUENCE;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    s_malloc_failed_index = 3;
    s_atiny_params.server_params.bootstrap_mode = BOOTSTRAP_FACTORY;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject != NULL), "get_security_object(...) failed");
    clean_security_object(pSecurityObject);

// WITH DTLS
    atiny_security_param_t  *iot_security_param = NULL;
    atiny_security_param_t  *bs_security_param = NULL;
    iot_security_param = &(s_atiny_params.security_params[0]);
    bs_security_param = &(s_atiny_params.security_params[1]);

    iot_security_param->server_port = (char*)"5684";
    bs_security_param->server_port = (char*)"5684";

#define IOT_PSK_VALUE_LENGTH    12
#define BS_PSK_VALUE_LENGTH     12

    char* endpoint_name_s = (char*)"11110001";
    char* endpoint_name_iots = (char*)"66667777";
    char* endpoint_name_bs = (char*)"22224444";
    unsigned char psk_iot_value[IOT_PSK_VALUE_LENGTH] = {0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33};  //0x33 -> 0x32
    unsigned char psk_bs_value[BS_PSK_VALUE_LENGTH] = {0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33};

    iot_security_param->psk_Id = endpoint_name_iots;
    iot_security_param->psk = (char*)psk_iot_value;
    iot_security_param->psk_len = IOT_PSK_VALUE_LENGTH;

    bs_security_param->psk_Id = endpoint_name_bs;
    bs_security_param->psk = (char*)psk_bs_value;
    bs_security_param->psk_len = BS_PSK_VALUE_LENGTH;

    s_malloc_failed_index = 3;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    s_malloc_failed_index = 4;
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject == NULL), "get_security_object(...) failed");

    cleanStub(&stub_info);
    
    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject != NULL), "get_security_object(...) failed");
    clean_security_object(pSecurityObject);
}

void TestObjectSecurity::test_security_get_uri(void)
{
    uint16_t serverId = 123;
    lwm2m_object_t *pSecurityObject = NULL;
    lwm2m_context_t context;

    memset(&context, 0, sizeof(lwm2m_context_t));

    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject != NULL && pSecurityObject->readFunc != NULL), "get_security_object(...) failed");

    char *uri = NULL;
    uri = security_get_uri(pSecurityObject, 1);
    TEST_ASSERT_MSG((uri == NULL), "security_get_uri(...) failed");
    uri = security_get_uri(pSecurityObject, 0);
    TEST_ASSERT_MSG((uri != NULL), "security_get_uri(...) failed");
    if(uri != NULL)
    {
        lwm2m_free(uri);
        uri = NULL;
    }

    clean_security_object(pSecurityObject);
}

void TestObjectSecurity::test_prv_security_read(void)
{
    uint16_t serverId = 123;
    lwm2m_object_t *pSecurityObject = NULL;
    lwm2m_context_t context;

    memset(&context, 0, sizeof(lwm2m_context_t));

    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject != NULL && pSecurityObject->readFunc != NULL), "get_security_object(...) failed");

    // read test
    int num = 0;
    lwm2m_data_t *pData = NULL;
    uint8_t ret = 0;
    ret = pSecurityObject->readFunc(0, &num, &pData, NULL, pSecurityObject);
    TEST_ASSERT_MSG((COAP_205_CONTENT == ret && pData != NULL), "prv_security_read(...) failed in object_security.c");
    if(pData != NULL)
    {
        lwm2m_data_free(num, pData);
    }

    clean_security_object(pSecurityObject);
}

void TestObjectSecurity::test_prv_security_write(void)
{
    uint16_t serverId = 123;
    lwm2m_object_t *pSecurityObject = NULL;
    lwm2m_context_t context;

    memset(&context, 0, sizeof(lwm2m_context_t));

    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject != NULL && pSecurityObject->readFunc != NULL), "get_security_object(...) failed");

    // read test
    int num = 0;
    lwm2m_data_t *dataArray = NULL;
    uint8_t ret = 0;

    dataArray = (lwm2m_data_t *)malloc(sizeof(*dataArray));
    memset(dataArray, 0, sizeof(*dataArray));
    TEST_ASSERT_MSG(dataArray != NULL, "dataArray malloc failed\r\n");

 
	dataArray->id = 0;
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_204_CHANGED == ret), "prv_security_write(...) failed in object_security.c");

    security_instance_t *targetP;
    targetP = (security_instance_t *)lwm2m_list_find(pSecurityObject->instanceList, 0);
    if (NULL == targetP)
    {
        return ;
    }
	lwm2m_free(targetP->uri);


	

    #if 0
	dataArray->id = 1;
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_400_BAD_REQUEST == ret), "prv_security_write(...) failed in object_security.c");

	dataArray->id = 2;
	stubInfo si_decode;
    setStub((void *)lwm2m_data_decode_int, (void *)stub_lwm2m_data_decode_int, &si_decode);
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");	
    cleanStub(&si_decode);


	dataArray->id = 3;
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");

	dataArray->id = 4;
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");

	dataArray->id = 5;
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");

	dataArray->id = 6;
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");

	dataArray->id = 7;
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");

	dataArray->id = 8;
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");

	dataArray->id = 9;
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");

	dataArray->id = 10;
	stubInfo si_decode_10;
    setStub((void *)lwm2m_data_decode_int, (void *)stub_lwm2m_data_decode_int, &si_decode_10);
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");	
    cleanStub(&si_decode_10);

	dataArray->id = 11;
	stubInfo si_decode_11;
    setStub((void *)lwm2m_data_decode_int, (void *)stub_lwm2m_data_decode_int, &si_decode_11);
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");	
    cleanStub(&si_decode_11);

	dataArray->id = 12;
	stubInfo si_decode_12;
    setStub((void *)lwm2m_data_decode_int, (void *)stub_lwm2m_data_decode_int, &si_decode_12);
	ret = pSecurityObject->writeFunc(0, 0, dataArray, pSecurityObject);
	TEST_ASSERT_MSG((COAP_205_CONTENT != ret), "prv_security_write(...) failed in object_security.c");	
    cleanStub(&si_decode_12);
#endif

	if(dataArray != NULL)
	{
		free(dataArray);
	}	
	lwm2m_list_free(pSecurityObject->instanceList);
	lwm2m_free(pSecurityObject);
    //clean_security_object(pSecurityObject);//test_clean_security_object

}


void TestObjectSecurity::test_prv_security_create(void)
{
    uint16_t serverId = 123;
    lwm2m_object_t *pSecurityObject = NULL;
    lwm2m_context_t context;

    memset(&context, 0, sizeof(lwm2m_context_t));

    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject != NULL && pSecurityObject->readFunc != NULL), "get_security_object(...) failed");

    // read test
    int num = 0;

    lwm2m_data_t *dataArray = NULL;
    uint8_t ret = 0;

    dataArray = (lwm2m_data_t *)malloc(sizeof(*dataArray));
    memset(dataArray, 0, sizeof(*dataArray));
    TEST_ASSERT_MSG(dataArray != NULL, "dataArray malloc failed\r\n");
	
    ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
    TEST_ASSERT_MSG((COAP_500_INTERNAL_SERVER_ERROR != ret), "prv_security_create(...) failed in object_security.c");

	////
    dataArray[0].id = LWM2M_SECURITY_BOOTSTRAP_ID;//1
	ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	dataArray[0].id = LWM2M_SECURITY_SECURITY_ID;//2
	stubInfo si_decodeinit;
	setStub((void*)lwm2m_data_decode_int,(void*)stub_lwm2m_data_decode_int,&si_decodeinit);
	ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	cleanStub(&si_decodeinit);
	
	dataArray[0].id = LWM2M_SECURITY_PUBLIC_KEY_ID;//3
	ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	dataArray[0].id = LWM2M_SECURITY_SERVER_PUBLIC_KEY_ID;//4
    ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	dataArray[0].id = LWM2M_SECURITY_SECRET_KEY_ID;//5
    ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	dataArray[0].id = LWM2M_SECURITY_SMS_SECURITY_ID;//6
    ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	dataArray[0].id = LWM2M_SECURITY_SMS_KEY_PARAM_ID;//7
	ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	dataArray[0].id = LWM2M_SECURITY_SMS_SECRET_KEY_ID;//8
	ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	dataArray[0].id = LWM2M_SECURITY_SMS_SERVER_NUMBER_ID;//9
    ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);

	dataArray[0].id = LWM2M_SECURITY_SHORT_SERVER_ID;//10
	setStub((void*)lwm2m_data_decode_int,(void*)stub_lwm2m_data_decode_int,&si_decodeinit);
    ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	
	dataArray[0].id = LWM2M_SECURITY_HOLD_OFF_ID;//11
    ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	
	dataArray[0].id = LWM2M_SECURITY_BOOTSTRAP_TIMEOUT_ID;//12
    ret = pSecurityObject->createFunc(0, 0, dataArray, pSecurityObject);
	cleanStub(&si_decodeinit);
	if(dataArray != NULL)
    {
        free(dataArray);
    }
	
	////
#if 0	//deal with memcheck
	
	if (targetP !=NULL && targetP->uri != NULL) lwm2m_free(targetP->uri);
#endif	//deal with memcheck end 
	security_instance_t *targetP;
	targetP = (security_instance_t *)lwm2m_list_find(pSecurityObject->instanceList, 0);

	lwm2m_free(targetP->publicIdentity);
	lwm2m_free(targetP->serverPublicKey);
	lwm2m_free(targetP->secretKey);
    clean_security_object(pSecurityObject);
}

void TestObjectSecurity::test_prv_security_delete(void)
{
    uint16_t serverId = 123;
    lwm2m_object_t *pSecurityObject = NULL;
    lwm2m_context_t context;

    memset(&context, 0, sizeof(lwm2m_context_t));

    pSecurityObject = get_security_object(serverId, &s_atiny_params, &context);
    TEST_ASSERT_MSG((pSecurityObject != NULL && pSecurityObject->readFunc != NULL), "get_security_object(...) failed");

    // read test

    lwm2m_data_t *dataArray = NULL;
    uint8_t ret = 0;

    dataArray = (lwm2m_data_t *)malloc(sizeof(*dataArray));
    memset(dataArray, 0, sizeof(*dataArray));
    TEST_ASSERT_MSG(dataArray != NULL, "dataArray malloc failed\r\n");

	
    ret = pSecurityObject->deleteFunc(0,pSecurityObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND != ret), "prv_security_delete(...) failed in object_security.c");
    if(dataArray != NULL)
    {
        free(dataArray);
    }


    clean_security_object(pSecurityObject);
}





/* Private functions --------------------------------------------------------*/
void TestObjectSecurity::setup()
{
    // init atiny_params
    atiny_security_param_t  *iot_security_param = NULL;
    atiny_security_param_t  *bs_security_param = NULL;
    memset(&s_atiny_params, 0, sizeof(atiny_param_t));
    s_atiny_params.server_params.binding = (char *)"UQ";
    s_atiny_params.server_params.life_time = 20;
    s_atiny_params.server_params.storing_cnt = 0;

    s_atiny_params.server_params.bootstrap_mode = BOOTSTRAP_FACTORY;

    iot_security_param = &(s_atiny_params.security_params[0]);
    bs_security_param = &(s_atiny_params.security_params[1]);

    iot_security_param->server_ip = (char *)"192.168.0.106";
    bs_security_param->server_ip = (char *)"192.168.0.106";

    iot_security_param->server_port = (char *)"5683";
    bs_security_param->server_port = (char *)"5683";

    iot_security_param->psk_Id = NULL;
    iot_security_param->psk = NULL;
    iot_security_param->psk_len = 0;

    bs_security_param->psk_Id = NULL;
    bs_security_param->psk = NULL;
    bs_security_param->psk_len = 0;
    
}

void TestObjectSecurity::tear_down()
{
    
}

