#include <gtest/gtest.h>

#include "codec_def.h"
#include "codec_app_def.h"
#include "codec_api.h"
#include "wels_common_basis.h"
#include "mem_align.h"
#include "ls_defines.h"

using namespace WelsDec;

class DecoderInterfaceTest : public ::testing::Test {
 public:
  virtual void SetUp() {
    int rv = WelsCreateDecoder (&m_pDec);
    ASSERT_EQ (0, rv);
    ASSERT_TRUE (m_pDec != NULL);
  }

  virtual void TearDown() {
    if (m_pDec) {
      WelsDestroyDecoder (m_pDec);
    }
  }
  //Init members
  void Init();
  //Uninit members
  void Uninit();
  //Mock input data for test
  void MockPacketType (const EWelsNalUnitType eNalUnitType);
  //Test Initialize/Uninitialize
  void TestInitUninit();
  //DECODER_OPTION_DATAFORMAT
  void TestDataFormat();
  //DECODER_OPTION_END_OF_STREAM
  void TestEndOfStream();
  //DECODER_OPTION_VCL_NAL
  void TestVclNal();
  //DECODER_OPTION_TEMPORAL_ID
  void TestTemporalId();
  //DECODER_OPTION_FRAME_NUM
  void TestFrameNum();
  //DECODER_OPTION_IDR_PIC_ID
  void TestIdrPicId();
  //DECODER_OPTION_LTR_MARKING_FLAG
  void TestLtrMarkingFlag();
  //DECODER_OPTION_LTR_MARKED_FRAME_NUM
  void TestLtrMarkedFrameNum();
  //DECODER_OPTION_ERROR_CON_IDC
  void TestErrorConIdc();
  //DECODER_OPTION_TRACE_LEVEL
  void TestTraceLevel();
  //DECODER_OPTION_TRACE_CALLBACK
  void TestTraceCallback();
  //DECODER_OPTION_TRACE_CALLBACK_CONTEXT
  void TestTraceCallbackContext();
  //Do whole tests here
  void DecoderInterfaceAll();


 public:
  ISVCDecoder* m_pDec;
  SDecodingParam m_sDecParam;
  SBufferInfo m_sBufferInfo;
  uint8_t* m_pData[3];
  char m_szBuffer[100]; //for mocking packet
  int m_iBufLength; //record the valid data in m_szBuffer
};

//Init members
void DecoderInterfaceTest::Init() {
  memset (&m_sBufferInfo, 0, sizeof (SBufferInfo));
  memset (&m_sDecParam, 0, sizeof (SDecodingParam));
  m_sDecParam.pFileNameRestructed = NULL;
  m_sDecParam.eOutputColorFormat = (EVideoFormatType) (rand() % 100);
  m_sDecParam.uiCpuLoad = rand() % 100;
  m_sDecParam.uiTargetDqLayer = rand() % 100;
  m_sDecParam.eEcActiveIdc = (ERROR_CON_IDC) (rand() & 3);
  m_sDecParam.sVideoProperty.size = sizeof (SVideoProperty);
  m_sDecParam.sVideoProperty.eVideoBsType = (VIDEO_BITSTREAM_TYPE) (rand() % 3);

  m_pData[0] = m_pData[1] = m_pData[2] = NULL;
  m_szBuffer[0] = m_szBuffer[1] = m_szBuffer[2] = 0;
  m_szBuffer[3] = 1;
  m_iBufLength = 4;
  CM_RETURN eRet = (CM_RETURN) m_pDec->Initialize (&m_sDecParam);
  ASSERT_EQ (eRet, cmResultSuccess);
}

void DecoderInterfaceTest::Uninit() {
  if (m_pDec) {
    CM_RETURN eRet = (CM_RETURN) m_pDec->Uninitialize();
    ASSERT_EQ (eRet, cmResultSuccess);
  }
  memset (&m_sDecParam, 0, sizeof (SDecodingParam));
  memset (&m_sBufferInfo, 0, sizeof (SBufferInfo));
  m_pData[0] = m_pData[1] = m_pData[2] = NULL;
  m_iBufLength = 0;
}

//Mock input data for test
void DecoderInterfaceTest::MockPacketType (const EWelsNalUnitType eNalUnitType) {
  switch (eNalUnitType) {
  case NAL_UNIT_SEI:
    //TODO
    break;
  case NAL_UNIT_SPS:
    //TODO
    break;
  case NAL_UNIT_PPS:
    //TODO
    break;
  case NAL_UNIT_SUBSET_SPS:
    //TODO
    break;
  case NAL_UNIT_PREFIX:
    //TODO
    break;
  case NAL_UNIT_CODED_SLICE:
    //TODO
    break;
  case NAL_UNIT_CODED_SLICE_IDR:
    //TODO
    break;
  default:
    //TODO
    break;

    m_szBuffer[m_iBufLength] = '\0';
    m_iBufLength++; //including '\0'

  }
}

//Test Initialize/Uninitialize
void DecoderInterfaceTest::TestInitUninit() {
  int iOutput;
  CM_RETURN eRet;
  //No initialize, no GetOption can be done
  m_pDec->Uninitialize();
  eRet = (CM_RETURN) m_pDec->GetOption (DECODER_OPTION_DATAFORMAT, &iOutput);
  EXPECT_EQ (eRet, cmInitExpected);
  //Initialize first, can get input color format
  m_sDecParam.eOutputColorFormat = (EVideoFormatType) 20; //just for test
  m_pDec->Initialize (&m_sDecParam);
  eRet = (CM_RETURN) m_pDec->GetOption (DECODER_OPTION_DATAFORMAT, &iOutput);
  EXPECT_EQ (eRet, cmResultSuccess);
  EXPECT_EQ (20, iOutput);

  //Uninitialize, no GetOption can be done
  m_pDec->Uninitialize();
  iOutput = 21;
  eRet = (CM_RETURN) m_pDec->GetOption (DECODER_OPTION_DATAFORMAT, &iOutput);
  EXPECT_EQ (iOutput, 21);
  EXPECT_EQ (eRet, cmInitExpected);
}

//DECODER_OPTION_DATAFORMAT
void DecoderInterfaceTest::TestDataFormat() {
  int iTmp = rand();
  int iOut;
  CM_RETURN eRet;

  //invalid input
  eRet = (CM_RETURN) m_pDec->SetOption (DECODER_OPTION_DATAFORMAT, NULL);
  EXPECT_EQ (eRet, cmInitParaError);

  //valid input
  eRet = (CM_RETURN) m_pDec->SetOption (DECODER_OPTION_DATAFORMAT, &iTmp);
  EXPECT_EQ (eRet, cmResultSuccess);
  eRet = (CM_RETURN) m_pDec->GetOption (DECODER_OPTION_DATAFORMAT, &iOut);
  EXPECT_EQ (eRet, cmResultSuccess);

  EXPECT_EQ (iOut, (int32_t) videoFormatI420);
}

//DECODER_OPTION_END_OF_STREAM
void DecoderInterfaceTest::TestEndOfStream() {
  //TODO
}
//DECODER_OPTION_VCL_NAL
void DecoderInterfaceTest::TestVclNal() {
  //TODO
}

//DECODER_OPTION_TEMPORAL_ID
void DecoderInterfaceTest::TestTemporalId() {
  //TODO
}

//DECODER_OPTION_FRAME_NUM
void DecoderInterfaceTest::TestFrameNum() {
  //TODO
}

//DECODER_OPTION_IDR_PIC_ID
void DecoderInterfaceTest::TestIdrPicId() {
  //TODO
}

//DECODER_OPTION_LTR_MARKING_FLAG
void DecoderInterfaceTest::TestLtrMarkingFlag() {
  //TODO
}

//DECODER_OPTION_LTR_MARKED_FRAME_NUM
void DecoderInterfaceTest::TestLtrMarkedFrameNum() {
  //TODO
}

//DECODER_OPTION_ERROR_CON_IDC
void DecoderInterfaceTest::TestErrorConIdc() {
  //TODO
}

//DECODER_OPTION_TRACE_LEVEL
void DecoderInterfaceTest::TestTraceLevel() {
  //TODO
}

//DECODER_OPTION_TRACE_CALLBACK
void DecoderInterfaceTest::TestTraceCallback() {
  //TODO
}

//DECODER_OPTION_TRACE_CALLBACK_CONTEXT
void DecoderInterfaceTest::TestTraceCallbackContext() {
  //TODO
}


//TEST here for whole tests
TEST_F (DecoderInterfaceTest, DecoderInterfaceAll) {

  //Initialize Uninitialize
  TestInitUninit();

  //AfterInitialize is OK, do the following tests
  Init();
  //DECODER_OPTION_DATAFORMAT
  TestDataFormat();
  //DECODER_OPTION_END_OF_STREAM
  TestEndOfStream();
  //DECODER_OPTION_VCL_NAL
  TestVclNal();
  //DECODER_OPTION_TEMPORAL_ID
  TestTemporalId();
  //DECODER_OPTION_FRAME_NUM
  TestFrameNum();
  //DECODER_OPTION_IDR_PIC_ID
  TestIdrPicId();
  //DECODER_OPTION_LTR_MARKING_FLAG
  TestLtrMarkingFlag();
  //DECODER_OPTION_LTR_MARKED_FRAME_NUM
  TestLtrMarkedFrameNum();
  //DECODER_OPTION_ERROR_CON_IDC
  TestErrorConIdc();
  //DECODER_OPTION_TRACE_LEVEL
  TestTraceLevel();
  //DECODER_OPTION_TRACE_CALLBACK
  TestTraceCallback();
  //DECODER_OPTION_TRACE_CALLBACK_CONTEXT
  TestTraceCallbackContext();

  //uninitialize
  Uninit();
}


