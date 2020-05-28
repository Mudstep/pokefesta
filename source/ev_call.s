@===================================================================
@
@	�X�N���v�g����̊֐��Ăяo���e�[�u����`
@
@	_2001_08_24 by tama
@
@===================================================================


@----------------------------------------------------------
@
@	�֐��e�[�u����`�p�̃}�N���錾
@
@----------------------------------------------------------
	.MACRO	INIT_CALL
.IFDEF	EV_CALL_S_GLOBAL
	.ALIGN
.ENDIF
DEF_CALL_COUNT	=	0
	.ENDM

	.MACRO	DEF_CALL	funcID , symname

.IFDEF	EV_CALL_S_GLOBAL			@ �B��̌��ł����e�[�u���͐������Ȃ�
.WORD	\symname
.ENDIF
\funcID			=	DEF_CALL_COUNT
DEF_CALL_COUNT 	=	(DEF_CALL_COUNT + 1)

	.ENDM

.MACRO	EV_CALL_LABEL	adrs
.IFDEF	EV_CALL_S_GLOBAL
	.GLOBAL	\adrs
\adrs:
.ENDIF
.ENDM


	INIT_CALL
	EV_CALL_LABEL		ScriptCallTable
	DEF_CALL	PC_KAIFUKU,		PokeAllKaifuku	@ add by matsuda 2001.09.07(��)
	DEF_CALL	SET_EXIT,		ForceExitSet	@ �����}�b�v�J�ڏ����i�o���̏�ł̂ݎg�p�j
	DEF_CALL	FORCE_COMM_IN,	AddCommMapChangeTask
	DEF_CALL	FORCE_COMM_OUT,	AddCommEndMapChangeTask
	DEF_CALL	SET_COMM_EXIT,	CommExitSet
	DEF_CALL	COMM_EXIT_WAIT,	CommExitWait

	DEF_CALL	BASE_SET,		BaseSet				@ 2001/10/04 nakahiro
	DEF_CALL	MY_BASE_CHECK,	MyBaseCheck			@ 2001/10/04 nakahiro
	DEF_CALL	CHANGE_BASE,	ChangeBaseMap		@ 2001/10/05 nakahiro
	DEF_CALL	BASE_MOVE,		BaseMove			@ 2001/10/05 nakahiro
	DEF_CALL	BASE_MOVE2,		BaseMove2			@ 2002/07/30 nakahiro
	DEF_CALL	WHOSE_BASE,		BaseBoardCheck		@ 2001/10/09 nakahiro
	DEF_CALL	B_ENTRY_CHECK,	CheckBaseEntry		@ 2001/10/16 nakahiro
	DEF_CALL	B_ENTRY_CHANGE,	ChangeBaseEntry		@ 2001/10/16 nakahiro
	DEF_CALL	B_RARNG_CHECK,	BaseRearrangeCheck	@ 2001/10/30 nakahiro
	DEF_CALL	B_ENTRY_INIT,	BaseEntryListInit	@ 2002/06/21 nakahiro
	DEF_CALL	ORUSUBAN_SET,	OrusubanBattle		@ 2001/12/18 nakahiro
	DEF_CALL	ORUSUBAN_CHECK,	OrusubanFlgCheck	@ 2001/12/18 nakahiro
	DEF_CALL	GOODS_OBJ_INIT, SetOBJGoods			@ 2001/12/20 nakahiro
	DEF_CALL	GOODS_OBJ_PUT,  PutOBJGoods			@ 2001/12/20 nakahiro
	DEF_CALL	GET_GOODS,      ObjGoodsReturn		@ 2001/12/21 nakahiro
	DEF_CALL	BASE_TYPE,		ScriptBaseTypeSet	@ 2001/12/25 nakahiro
	DEF_CALL	ORUSUBAN_CODE,	SetOrusubanOBJCode	@ 2001/12/26 nakahiro
	DEF_CALL	GET_GOODS2,     ObjGoodsReturn2		@ 2001/12/21 nakahiro
	DEF_CALL	BASE_MAKE,      MakeBaseFinal		@ 2002/03/14 nakahiro
	DEF_CALL	ORUSUBAN_FLAG	OrusubanBtlFlgSet	@ 2002/07/20 nakahiro
	DEF_CALL	BASE_PC_OFF_ANM	BasePCOffAnm		@ 2002/08/07 nakahiro

	DEF_CALL	COMM_MIXER,		CommMixStart	@ �܂���ʐM�J�n

	DEF_CALL	CONNECT_BATTLE,	ConnectBattleSio	@�ʐM�ΐ��t
	DEF_CALL	CONNECT_TRADE,	ConnectTradeSio		@�ʐM������t
	DEF_CALL	CONNECT_RECORD,	ConnectRecordSio	@���R�[�h�R�[�i�[��t
	DEF_CALL	COMM_END,		PokemonMultiSioEnd	@�ʐM�I������
	DEF_CALL	COMM_BATTLE,	AddCommBattleStartTask		@�ʐM�ΐ�J�n
	DEF_CALL	COMM_TRADE,		AddCommTradeStartTask		@�ʐM�����J�n
	DEF_CALL	COMM_RECORD,	ChangeToKoukan		@���R�[�h�����J�n
	DEF_CALL	COMM_SAVE,		ConnectSave			@�ʐM�O�̃Z�[�u����
	DEF_CALL	CONNECT_BLEND,	ConnectBlendSio		@������ϼ�ݒʐM�m�F 02/02/27 nohara
	@DEF_CALL	GET_COMM_NUM,	GetConnectSioMenberNum@�ʐM�l���擾 02/02/27 nohara
	DEF_CALL	SET_BLEND_COMM_NAME,SetBlendCommName@������ϼ�ݒʐM�Ŏ擾�������O 02/02/27 nohara
	DEF_CALL	SET_BLEND_COMM_OBJ,SetBlendCommOBJ	@������ϼ�ݒʐM��OBJ�z�u 02/02/27 nohara

	DEF_CALL	TEMOTI_POKE_PUSH,	BackUpPokeParaMine		@�莝���|�P�����ޔ�
	DEF_CALL	TEMOTI_POKE_POP,	RecoverPokeParaMine		@�莝���|�P�������A
	DEF_CALL	BATTLE_POKE_SEL,	SelectBattleMonsStart	@�莝���|�P�����I��
	DEF_CALL	COMM_TR_CARD,	CommTrainerCard			@�g���[�i�[�J�[�h�\��

	DEF_CALL	GET_SEED_PARAM,	SeedScriptParam	@ �^�l�̖��O�Ȃǂ��擾
	DEF_CALL	SEED_CHOICE,	SeedChoice		@ �^�l��I��
	DEF_CALL	SEED_SET,		SeedSet			@ �^�l���Z�b�g
	DEF_CALL	SEED_GET,		SeedGet			@ �^�l���Ƃ�
	DEF_CALL	SEED_CLEAR,		SeedClear		@ �c����������Ԃɂ���
	DEF_CALL	SEED_WATER,		SeedWaterSet	@ �����
	DEF_CALL	CHK_SEED_POCKET,SeedPocketCheck	@ �^�l�|�P�b�g�̃`�F�b�N
	DEF_CALL	CHK_RAM_SEED,	CheckRamSeedExist@ RAM�؂̎������݂��邩�`�F�b�N

	DEF_CALL	TR_TYPE_GET,	GetTrainerType
	DEF_CALL	TR_BEFORE_MSG,	PutTrainerBeforeFightMsg
	DEF_CALL	TR_NOFIGHT_MSG,	PutTrainerNotFightMsg
	DEF_CALL	TR_FLAG_CHECK,	CheckNowTrainerFlag
	DEF_CALL	TR_MOVE_CTRL,	AddTrainerMoveCtrlTask
	DEF_CALL	TR_BGM_SET,		SetTrainerBGM
	DEF_CALL	TR_REVENGER_CHK,IsRevengerCheck
	DEF_CALL	TR_REV_FLAG_CHK,CheckRevengerFlag
	DEF_CALL	TR_REV_FIGHT,	FieldRevengeTrainerFightStart

	DEF_CALL	PASO_BOX_MENU,	BoxMenuStart
	DEF_CALL	POKE2VS2_CHK,	AnswerPoke2vs2Check	@2vs2�ΐ�\���ǂ����̃`�F�b�N

	DEF_CALL	RETURN_TVCELL,  ReturnTVCell		@ TV�̃Z�������ɖ߂�
	DEF_CALL	TV_BROADCAST,	TVBroadCastSelect	@ TV����
	DEF_CALL	CM_BROADCAST,	CMBroadCastSet		@ TV����(CM)
	DEF_CALL	CHK_TV_WATCH_FLAG,TVDataWatchFlagOnChk	@ �����o����TV�f�[�^�����邩
	DEF_CALL	GET_TV_TYPE,	ScrTVTypeGet		@ datatype �擾
	DEF_CALL	LETTER_TYPE_CHK,LetterTypeSelectChk	@ �莆�^�̃f�[�^�쐬�`�F�b�N
	DEF_CALL	TV_DATA_SET,	TVDataSetSelect		@ �f�[�^�Z�b�g
	DEF_CALL	TOP_POKE_NICK_CHK,ScrTopPokeNickNameChk@ �擪�|�P�����Ƀj�b�N�l�[�����Ă邩
	DEF_CALL	SET_TV_CON_TYPE,ScrTVContestTypeGet	@ �ۑ����Ă���R���e�X�g�̃^�C�v�Z�b�g
	DEF_CALL	CHK_GENERATING,	ScrGeneratingBroadCastChk@ ��ʔ����㏑���`�F�b�N
	DEF_CALL	CHK_TV_TYPE,	ScrTVDataTypeChk	@ �w�肵���s�u�f�[�^�����łɂ��邩
	DEF_CALL	CHK_HOME_TV,	TVMyHomeChk			@ ����s�u������邩
	DEF_CALL	SET_TV_PAPAMAMA,TVNoDataMsgSet		@ �p�p�}�}�Z�b�g
	DEF_CALL	TV_BROADCAST_INIT,TVBroadCastInit	@ �����O�̏�����

@	DEF_CALL	GET_LISTPOKE,   GetListChoicePoke@ �R���e�X�g
	DEF_CALL	GET_CHAMPENTNO, GetChampEntryNumber@ 
	DEF_CALL	GET_TUUSINENTNO,GetEntryNo		@ 
	DEF_CALL	GET_CHAMP_OBJID,GetChampObjID	@ 
	DEF_CALL	GET_CHAMP_NAME, GetChampBreederName@ 
	DEF_CALL	GET_CHAMP_POKENAME,GetChampPokeName@ 
	DEF_CALL	SET_BRD_DATA,	SetBrdName		@ 
	DEF_CALL	GET_PRIMARYRANK,GetPrimaryRank	@ 
@	DEF_CALL	GET_CONRANK,	GetContestRank	@ 
@	DEF_CALL	GET_CONTYPE,	GetContestType	@ 
	DEF_CALL	SET_BRD_OBJ,	SetBreederObj	@ 
	DEF_CALL	CONSANKA_CHECK,	ContestSankaCheck
	DEF_CALL	GET_BP_BRD_DATA,GetBPBreederData
	DEF_CALL	SET_CON_SIO_OBJ,ContestSioObjChrSet
	DEF_CALL	GET_BP_NUM,		GetBPNum		@ 
	DEF_CALL	CHK_RIBBON,		RibbonCheck		@ 
	DEF_CALL	SET_RIBBON,		RibbonSet		@ 
	DEF_CALL	CHK_D_CON,		CheckDebugContestFlag
	DEF_CALL	CHK_BROMIDE_RIBBON,	BromideRibbonCheckSet
	DEF_CALL	CHK_SIO_CONTEST,	ConnectContestSio	@�R���e�X�g�̒ʐM�ڑ��`�F�b�N
	
	DEF_CALL	SAVE_INTERFACE,	ReportSequenceStart

	DEF_CALL	ZYOURO_ANIME,	AddZyouroAnimeTask

	DEF_CALL	KAIWA_INPUT,	KaiwaInput	@ �ȈՉ�b���͉��
	DEF_CALL	GET_KAIWA_MSG,	KaiwaCodeToMsgBuffer  @ �w���b�R�[�h�𕶎���ϊ��A�o�b�t�@�i�[


	DEF_CALL	GET_OYAJI_TYPE, OyajiScr_GetType	@ ���₶�^�C�v�擾
	DEF_CALL	OYAJI_SING_CHK, OyajiScr_SingerCheck@ �V���K�[�t���O�`�F�b�N
	DEF_CALL	OYAJI_SING_DONE,OyajiScr_SingerDone	@ �V���K�[�̎��ύX
	DEF_CALL	OYAJI_NAUI_CHK, OyajiScr_NowCheck	@ �i�E�C�t���O�`�F�b�N
	DEF_CALL	OYAJI_NAUI_DONE,OyajiScr_NowDone	@ �i�E�C���t����������Z�b�g
	DEF_CALL	OYAJI_NAUI_WORD,OyajiScr_NowWord	@ �i�E�C�V�������t�������
	DEF_CALL	SET_OYAJI_SING_WORD,OyajiScr_AddSingTask@ �̕\��
	DEF_CALL	SET_OBJ_OYAJI,	SetOBJOyajiType		@ OBJCHRWORK1�ɃZ�b�g
	DEF_CALL	OYAJI_MARCHEN_WORD,OyajiScr_MarchenWord	@ �V�����Z���t��buf�ɃR�s�[
	DEF_CALL	OYAJI_MARCHEN_CHK,OyajiScr_MarchenCheckWord@ �V�����Z���t�����邩chk

	DEF_CALL	OYAJI_DEN_GET_CNT	,OyajiScr_LegendGetCount	@�ł񂹂̐����擾
	DEF_CALL	OYAJI_DEN_MSGPUT	,OyajiScr_LegendMsg			@�ł񂹂l�r�f�\��
	DEF_CALL	OYAJI_DEN_WINDOW	,OyajiScr_LegendWindow		@�ł񂹂I���E�B���h�E
	DEF_CALL	OYAJI_DEN_RENEW_CHK	,OyajiScr_LegendRenewCheck	@�ł񂹂X�V�`�F�b�N
	DEF_CALL	OYAJI_DEN_NEW_CHK	,OyajiScr_LegendNewCheck	@�ł񂹂V�K�`�F�b�N
	DEF_CALL	OYAJI_DEN_ADD_CHK	,OyajiScr_LegendAddCheck	@�ł񂹂ǉ��`�F�b�N

	DEF_CALL	OYAJI_RECYCLE_SELECT,InitRecycleWinSelect		@ �O�b�Y�I�������ݒ�
	DEF_CALL	OYAJI_TRADE_CHANGE,  TradeOyajiTradeCheck		@ �O�b�Y����������
	DEF_CALL	OYAJI_TRADE_GOODS_CHECK, TradeOyajiGoodsMaxCheck   @ �O�b�Y�������Ă��邩
	DEF_CALL	OYAJI_TRADE_GENRE_CHECK, TradeOyajiGoodsGenreCheck @ �O�b�Y�̍��ڃ`�F�b�N
	DEF_CALL	OYAJI_TRADE_MY_GOODS_SELECT, TradeOyajiMyGoodsSelect @ ��������O�b�Y��I��
	DEF_CALL	OYAJI_TRADE_ADD,		TradeOyageGoodsTarade @ �O�b�Y������


	DEF_CALL	KINOKOKO_OYAJI_SIZE_MSGSET,	KinokokoSizeOyajiRecordInfoSet		@���������I���W(�L�m�R�R):MSG�Z�b�g
	DEF_CALL	KINOKOKO_OYAJI_SIZE_CHECK,	KinokokoSizeOyajiCheck				@���������I���W(�L�m�R�R)�F�|�P�����`�F�b�N
	DEF_CALL	NAMAZO_OYAJI_SIZE_MSGSET,	NamazoSizeOyajiRecordInfoSet		@���������I���W(�i�}�]):MSG�Z�b�g
	DEF_CALL	NAMAZO_OYAJI_SIZE_CHECK,	NamazoSizeOyajiCheck				@���������I���W(�i�}�])�F�|�P�����`�F�b�N

	DEF_CALL	NICKNAME_CHK,	ScrNickNameChangeChk@ �j�b�N�l�[�����Ă邩
	DEF_CALL	SET_MY_POKENAME,ScrSetMyPokeNick	@ �j�b�N�l�[����buf��set
	DEF_CALL	ID_CHK,			ScrTrainerIDCmp		@ ID��r

	DEF_CALL	SET_POPWORD,	PopWord_SetBuffer	@ ���s��buf�ɃZ�b�g
	DEF_CALL	POPWORD_COMING_CHK,PopWord_CheckComingWord@ ���s���ւ�肻�����`�F�b�N
	DEF_CALL	RAND_WORD_COPY,	KaiwaCopyRandomWord	@ �P��������_����buf�ɃR�s�[
	DEF_CALL	GET_POPWORD_VALUE,	PopWord_GetValue	@ ���s��̍��v3bit

	DEF_CALL	CHANGE_CNVBTN,	ChangeCnvbtn		@ �֗��{�^���̌���

	DEF_CALL	GET_POKE_COUNT,	 PokeCountMineGet	@ �莝���̃|�P������
	DEF_CALL	GET_POKE_COUNT2, Box_CountMinePoke	@ �^�}�S���������莝���̃|�P������
	DEF_CALL	GET_POKE_COUNT3, Box_CountMinePokeEx@ �^�}�S�ƕm�����������莝���̃|�P������

	DEF_CALL	ART_COND_CHK,	ArtMuseumCondCheck	@ Ͻ���ō����_�D����
	DEF_CALL	SET_NEW_ART,	ArtMuseumPortraitSet@ �V�����G���Z�b�g
	DEF_CALL	ART_CHK,		ArtMuseumPortraitCheck@ ���łɏ����Ă��邩
	DEF_CALL	ART_COUNT_CHK,	ArtMuseumCountCheck	@ �G�̖����𒲂ׂ�
	DEF_CALL	PICTURE_PUT,	PortraitPicturePut	@ 

	DEF_CALL	GYM03_DOOR_CHANGE_CELL,ScrGYM03DoorChangeCell@ �h�A�̃Z����ύX
	DEF_CALL	GYM03_SWITCH_CHANGE_CELL,ScrGYM03SwitchChangeCell@ �X�C�b�`�̃Z����ύX
	DEF_CALL	MSGEXPANDBUF,	ScrMsgExpandBufferSet@ FieldMsgSet(MsgExpandBuffer)
	DEF_CALL	MAKE_MAP_BG,	MakeMapBG			@ ��аVram �W�J
	DEF_CALL	GET_MY_POS,		ScrGetMyPos			@ ���@�̈ʒu���擾����
	DEF_CALL	GYM03_ALL_CHANGE_CELL,ScrGYM03AllChangeCell@ �L���Z�c�W���̃Z�������ׂĕύX

	DEF_CALL	GYM05_DOOR_ANIME,	ScrGYM05DoorAnimeWait	@ �h�A���A�j��������
	DEF_CALL	GYM05_DOOR_CHG_CELL,ScrGYM05DoorChangeCell	@ �h�A�̃Z����ύX

	@DEF_CALL	C106_POKE_CHG_CELL,	ScrC106ChangeCell	@ �z�G���R�̃Z����ύX
	DEF_CALL	GET_ID_NO,			ScrGetID			@ ID�擾(���͉��ꌅ)
	DEF_CALL	SET_SEX_NAME_01,	ScrSetName01		@ ���o�����A���Z�����Z�b�g(��)
	DEF_CALL	SET_SEX_NAME_02,	ScrSetName02		@ �ނ����A�ނ��߃Z�b�g(��)
	DEF_CALL	WK_FLAG_SET,		ScrWKFlagSet		@ wk�ɓ����Ă���l��flagno�Ƃ��ēn��

	DEF_CALL	SET_ROPEWAY_ID,	ScrSetRopewayMapID		@ ���[�v�E�F�C�̐ڑ�����Z�b�g
	DEF_CALL	ROPEWAY_START,	DemoRopewayCall			@ ���[�v�E�F�C�f���Ăяo��

	DEF_CALL	BGM_START,			MapBGM_Start
	DEF_CALL	TIME_SET_WINDOW,	TimeSetWindow		@ ���v���킹��
	DEF_CALL	TIME_CHK_WINDOW,	ScrTokeiLookInit	@ ���Ԋm�F
	DEF_CALL	FIRST_BATTLE,		PokeSelEventStart
	DEF_CALL	CAPUTURE_DEMO,		StartCaptureDemoEncountEvent


	DEF_CALL	CHANGE_NICKNAME,ScrNickNameChange	@ �������f���p���O���͉�ʂ��Ă�
	DEF_CALL	NAME_POKE_LIST,	NamePokeListInit	@ �������f���p�߹��ؽĉ�ʂ��Ă�

	DEF_CALL	CHK_CUBECASE,	CheckCube			@ �L���[�u�P�[�X�ɋ󂫂����邩

	DEF_CALL	START_BLENDER,	BlendMachineTitleInit@ �^�l�u�����_�[���Ă�

	DEF_CALL	START_ROULETTE,	RouletteSet			@ ���[���b�g���Ă� 02/05/10 by nakahiro

	DEF_CALL	CHK_FAN_BIT,	ScrFanBitChk		@ �����̃t�@�����`�F�b�N
	DEF_CALL	GET_FAN_COUNT,	FanNoGet			@ �t�@���̐l�����擾
	DEF_CALL	SET_FAN_NAME,	ScrFanNameSet		@ �t�@���̖��O���Z�b�g
	DEF_CALL	SET_FAN_TIME,	FanCommBattleTimeSet@ ���Ԃ��Z�b�g
	DEF_CALL	CHK_FAN_TIME,	FanTimeChk			@ ���Ԃ��`�F�b�N
	DEF_CALL	SET_FAN_FLAG,	FanStartFlagOn		@ �t���O�I��
	DEF_CALL	FAN_START,		FanStart			@ �t�@�����X�^�[�g
	DEF_CALL	ADD_FAN_POINT,	ScrFanPointAdd		@ �t�@���|�C���g����

	DEF_CALL	IWA_ENCOUNT,	IwakudakiEncount	@ ��ӂ��G���J�E���g

	DEF_CALL	GET_CAMERA_VS,	TVCameraBattleGet	@ �퓬�񐔂��擾
	DEF_CALL	SET_CAMERA_DATA,TVCameraDataSet		@ �f�[�^�Z�b�g
	DEF_CALL	SET_CAMERA_VS,	TVCameraBattleDataSet@ �퓬���̃f�[�^�Z�b�g
	DEF_CALL	CAMERA_BROADCAST,TVCameraBroadCastSet@ ����
	DEF_CALL	CHK_TV_CAMERA,	TVCameraBroadCastChk@ �����ł��邩
	DEF_CALL	SET_CAMERA_KAIWA,TVCameraKaiwaSet	@ �O��̊ȈՉ�b�Z�b�g
	DEF_CALL	CHK_OLD_CAMERA,	TVCameraOldFlagChk	@ �O��̃t���O���`�F�b�N����
	DEF_CALL	GET_CAMERA_OBJID,TVCameraObjIDGet	@ �C���^�r���A�[��OBJ�w����擾����

	@DEF_CALL	TEST_NOHARA,	TestNoharaCall		@ �e�X�g�p
	DEF_CALL	GET_WIN_LOSE,	ScrGetWinLoseFlag			@ �퓬��Ԃ��擾����

	DEF_CALL	GET_SODATE_NAME,SodateyaPokeNameSet			@ ��ĉ��ɂ���|�P�����̖��O���Z�b�g����
	DEF_CAll	GET_SODATEYA_ZIISAN,SodateyaZiisannCheck	@ ��ĉ��̏�ԃ`�F�b�N
	DEF_CALL	DEL_SODATEYA_EGG	DelSodateyaTamago		@ ��ĉ��ɂ���^�}�S������
	DEF_CALL	GET_SODATEYA_EGG	PokeTamagoDel			@ ��ĉ�����^�}�S��Ⴄ
	DEF_CALL	MSG_SODATEYA_AISHOU	SodateyaPokeAffinityMsgSet	@ ��ĉ��ɂ���|�P�����̑�����������Z�b�g
	DEF_CALL	MSG_AZUKE_SET	SodateyaAzukeName			@ ��ĉ��ɂ�������|�P�����̖��O���Z�b�g
	DEF_CALL	SET_SODATEYA_POKE	PokeSodateSet			@ �|�P��������������
	DEF_CALL	SODAYE_POKELIST	SodatePokeListCall			@ ��ĉ��p�̃|�P�������X�g�Ăяo��
	DEF_CALL	HIKITORI_LIST	SodateyaSelectPoke			@ ��ĉ�����������Ƃ��Ƀ|�P�����I��
	DEF_CALL	SODATE_POKE_LEVEL_STR SodatePokeLevelSet	@ ��ĉ��ɂ���|�P�����̃��x�����𕶎���ŃZ�b�g
	DEF_CALL	HIKITORI_RYOUKIN PokeHikitoriRyoukin		@ �w�肵���|�P�����ƈ�ĉ��������i�[
	DEF_CALL	HIKITORI_POKE	PokeHikitori				@ �|�P�������������
	DEF_CALL	TAMAGO_HUKA		SodateTamagoHuka			@ �^�}�S���|�P�����ɛz������
	DEF_CALL	TAMAGO_DEMO		TamagoDemoCall				@�^�}�S���܂�f��
	DEF_CALL	SIOMAIL_CHECK	SodateyaMailCheck			@ ���R�[�h�R�[�i�[���烁�[�������Ă��邩

	DEF_CALL	SCORE_WINDOW,	PutTaisenScoreWindow
	DEF_CALL	COMP_MY_GOLD	CheckMyGold					@ScriptParameter0���������������Ă��邩
	DEF_CALL	SUB_MY_GOLD		SubMyGold					@Scriptparameter0�~��������������
	DEF_CALL	POISON_MSG,		PoisonDead
	DEF_CALL	GAME_OVER,		GameOver

	DEF_CALL	GYM_DOOR_INIT,	DoorObjInit
	DEF_CALL	GYM_DOOR_SET,	DoorObjSet

	DEF_CALL	CRUISE_START,	ShipStepCountStart			@�D���f�����[�h�J�n
	DEF_CALL	CRUISE_END,		ShipStepCountEnd			@�D���f�����[�h�I��

	DEF_CALL	SAFARI_START,	  SafariModeStart
	DEF_CALL	SAFARI_END,		  SafariModeEnd
	DEF_CALL	SAFARI_CUBECHECK, SafariCubeTableCheck		@ �L���[�u�u����ɃL���[�u���u����Ă��邩
	DEF_CALL	SAFARI_CUBECASE,  SafariTblCubeCaseCall		@ �L���[�u�u����ł̃P�[�X�Ăяo��

	DEF_CALL	MABOROSHI_CHECK,	Maboroshi_Check			@�}�{���V�W�}�̏o���`�F�b�N
	DEF_CALL	ASASE_TIDE_CHECK,	AsaseTideCheck			@�󐣂̓��A�̖��������`�F�b�N

	DEF_CALL	HYOUKA_INIT,	ZukanHyouka_Init			@�}�ӕ]���V�X�e��������
	DEF_CALL	GET_ZUKAN_NUM,	ScrZukanNumGet				@�}��
	DEF_CALL	SET_HYOUKA_MSG,	SetZukanHyoukaMsg			@�}�ӕ]�����b�Z�[�W�Z�b�g

	DEF_CALL	ADD_PASO_ANIME,	PasoAnimeTaskAdd			@ �p�\�R���A�j���ǉ�
	DEF_CALL	DEL_PASO_ANIME,	PasoAnimeTaskDel			@ �p�\�R���A�j���폜
	DEF_CALL	SET_NOW_FLOOR,	ElevatorNowFloorSet			@ �G���x�[�^����
	DEF_CALL	ADD_NOTE_ANIME,	NoteAnimeTaskAdd			@ �m�[�g�A�j���ǉ�
	DEF_CALL	DEL_NOTE_ANIME,	NoteAnimeSeqDel				@ �m�[�g�A�j���폜

	DEF_CALL	WAZAOSIE_POKELIST, ManiaPokeListInit		@ �Z�����}�j�A�|�P���X�g�Ăяo��
	DEF_CALL	ST_WAZAWASURE,     WazaWasureCall			@ �Z�Y���ʌĂяo���i�S�ȉ��j
	DEF_CALL	WAZA_WASURE_MAIN,  WazaWasureMain			@ �Z�Y�ꃁ�C������
	DEF_CALL	WAZA_WASURE_NAME,  WazaWasureStrSet			@ ��������Z�b�g
	DEF_CALL	WAZA_WASURE_CHECK, WazaWasureCheck			@ �o���Ă���Z�̌��𒲂ׂ�
	DEF_CALL	WAZA_OSHIE_MANIA,  WazaOshieManiaCall		@ ���U�������}�j�A��ʂ֔��

	DEF_CALL	SET_CYCLE_RECORD,	CyclingRoadRecordSet	@ �T�C�N�����O���[�h�L�^�Z�b�g
	DEF_CALL	START_CYCLE_ROAD,	CyclingRoadEventStart	@ �T�C�N�����O���[�h�C�x���g�X�^�[�g
	DEF_CALL	CHK_CYCLE_ROAD,		CyclingRoadGateChk		@ ���]�ԏ���Ă��邩�`�F�b�N
	DEF_CALL	GOAL_CYCLE_ROAD,	CyclingRoadGoal			@ �S�[������
	DEF_CALL	BEFORE_MAP_CYCLE,	CyclingRoadBeforeMapChk	@ �O�̃}�b�v�����ăV�[����߂�
	DEF_CALL	GET_NATUKIDO_LEVEL,	GetNatukidoLevel	@ �O�̃}�b�v�����ăV�[����߂�
	DEF_CALL	BATTLE_TOWER_INIT,	BattleTowerWorkInit	@ �o�g���^���[�f�[�^�̏�����
	DEF_CALL	BATTLE_TOWER_TRAINER_SET,	BattleTowerTrainerSet	@ �o�g���^���[�̃g���[�i�[�����߂�
	DEF_CALL	BATTLE_TOWER_POKE_CHECK,	BattleTowerPokeCheck	@ �o�g���^���[�̎Q���|�P�����`�F�b�N
	DEF_CALL	BATTLE_TOWER_KAIWA_GET,	BattleTowerKaiwaGet	@ �o�g���^���[�̊ȈՉ�b���擾����
	DEF_CALL	BATTLE_CARD_E_KAIWA_GET,	BattleCardEKaiwaGet	@ �J�[�h���̊ȈՉ�b���擾����
	DEF_CALL	BATTLE_EVENT_FIGHT,	BattleEventFight		@ ���낢��ȃC�x���g�퓬���Ăяo��
	DEF_CALL	BATTLE_TOWER_WORK_SET,	BattleTowerWorkSet	@ �o�g���^���[�p�̃��[�N�ɒl���Z�b�g
	DEF_CALL	BATTLE_TOWER_WORK_GET,	BattleTowerWorkGet	@ �o�g���^���[�p�̃��[�N����l���Q�b�g
	DEF_CALL	BATTLE_TOWER_MONS_SEL,	BattleTowerMonsSel	@ �o�g���^���[�̑I���|�P�����Z�b�g
	DEF_CALL	BATTLE_TOWER_SAVE,	BattleTowerSave		@ �o�g���^���[�̕ۑ�
	DEF_CALL	BATTLE_TOWER_RESET,	BattleTowerReset	@ �\�t�g���Z�b�g
	DEF_CALL	BATTLE_TOWER_ITEM_SET,	BattleTowerItemSet	@ �o�g���^���[�ł̌i�i�Q�b�g
	DEF_CALL	BATTLE_TOWER_ITEM_GET,	BattleTowerItemGet	@ �o�g���^���[�ł̌i�i�Q�b�g
	DEF_CALL	BATTLE_TOWER_RIBBON_GET,	BattleTowerRibbonGet	@ �o�g���^���[�ł̃��{���Q�b�g
	DEF_CALL	BATTLE_TOWER_POKE_SEL,	SelectBattleTowerMonsStart	@�o�g���^���[�ł̎莝���|�P�����I��
	DEF_CALL	BATTLE_CARD_E_DATA_CHECK,	BattleCardECheckSumCheck	@ �J�[�h���f�[�^�̃`�F�b�N�T���`�F�b�N
	DEF_CALL	BATTLE_TOWER_RENSHOU,	BattleTowerRenshouGet	@ �o�g���^���[�ł̘A�����Q�b�g
	DEF_CALL	SORT_SELECT_POKEMON,	SortSelectPokemon	@ �I������3�̂��莝���ɃZ�b�g
	DEF_CALL	MY_ROOM_MY_PC,		MyPC_MyRoomCall		@ ������PC�������̕�������Ăяo��
	DEF_CALL	GLOBAL_PC_MY_PC,	MyPC_GlobalPCCall	@ ������PC�������p�[������Ăяo��
	DEF_CALL	TOWN_MAP,			ScrInfoMapInit		@ �^�E���}�b�v�Ăяo��
	DEF_CALL	KOUKAN_JUNBI,		EventKoukanJunbi	@ �����C�x���g�p�̃|�P�����f�[�^����
	DEF_CALL	KOUKAN_POKE,		EventKoukanTest		@ �����C�x���g�̃|�P�����Ɠ��ꂩ����
	DEF_CALL	KOUKAN_POKEDEMO,	KoukanDemoOfflineCall @�����C�x���g�̃f���J�n
	DEF_CALL	TEMOTI_MONSNO,		PokeMineMonsNumber  @ PARA1�̎莝���|�P�����̎�ޔԍ���Ԃ�

	DEF_CALL	GET_START_WEEK,		ScrGetWeekFromGameStart @ �Q�[�����߂Ă��牽�T�Ԃ��������擾

	DEF_CALL	GET_LOT_NUM,		GetPokeLotNumber	 @ �|�P���������̓��I�ԍ��擾
	DEF_CALL	CHK_LOT_NUM,		CheckPokeLotNumber	 @ �|�P���������̓��I�`�F�b�N
	DEF_CALL	BLENDMACHINE_SCORE	PutBlendMachineScoreWindow	@���̂݃u�����_�[�̑��x�����L���O

	DEF_CALL	KARAKURI_HIDE_ITEM_ON,	KarakuriHideItemOn	 @ �J���N���B���A�C�e������(�o��)
	DEF_CALL	KARAKURI_HIDE_ITEM_OFF,	KarakuriHideItemOff	 @ �J���N���B���A�C�e������(����)

	DEF_CALL	PASO_MENU_WRITE,	AddScriptPasoMenu	 @ �p�\�R�����j���[�\��
	DEF_CALL	DENDOU_IRI,			ScrDendouLookInit	 @ �ł�ǂ�����\��

	DEF_CALL	HYOUSYOU_START,		ScrHyousyouInit		 @ �\����\��

	DEF_CALL	DAISUKI_STYLE_CHECK,	DaisukiClubStyleCheck	@ ���������N���u�p�������悳�`�F�b�N
	DEF_CALL	DAISUKI_BEAUTIFUL_CHECK,	DaisukiClubBeautifulCheck	@ ���������N���u�p�������`�F�b�N
	DEF_CALL	DAISUKI_CUTE_CHECK,	DaisukiClubCuteCheck	@ ���������N���u�p���킢���`�F�b�N
	DEF_CALL	DAISUKI_CLEVER_CHECK,	DaisukiClubCleverCheck	@ ���������N���u�p�����`�F�b�N
	DEF_CALL	DAISUKI_STRONG_CHECK,	DaisukiClubStrongCheck	@ ���������N���u�p�����܂����`�F�b�N

	DEF_CALL	SHIP_EVENT_START,	ShipEventModeStart

	DEF_CALL	SOFT_RESET,			PM_SoftReset		@ �\�t�g���Z�b�g
	DEF_CALL	GAME_CLEAR,			ScrGameClear		@ �G���f�B���O��

	DEF_CALL	ELEVATOR_MOVE,		AddElevatorMoveTask	@ �G���x�[�^�h�ꏈ���ǉ�
	DEF_CALL	KAZANBAI_MENU,		AddKazanbaiMenuTask	@ �ΎR�D�̃A�C�e�����j���[�\��

	DEF_CALL	SET_MAP_SCROOL,		ScrMapScroolSet		@ �X�N���[���p�_�~�[OBJ�ǉ�
	DEF_CALL	RESET_MAP_SCROOL,	ScrMapScroolReset	@ �X�N���[���A�N�^�[�N���A

	DEF_CALL	GET_LIKE_CUBE,		ScrGetLikeCube		@ ���i�ɂ���čD���ȃL���[�u���擾
	DEF_CALL	GET_BASE_MAPNO,		ScrGetMyBaseMapNo	@ �����̔閧��n�̂���}�b�v���擾

	DEF_CALL	TENJI_POKE_RETU,	TenjiPokeRetuChk	@ �|�P�����̕��т���������
	DEF_CALL	TENJI_WAIT_START,	TenjiWaitEventStart	@ �L�[���������ɑ҂C�x���g
	DEF_CALL	START_JEWEL_EFFECT,	StartJewelEffect	@ ��ʃG�t�F�N�g�J�n
	DEF_CALL	FINISH_JEWEL_EFFECT,FinishJewelEffect	@ ��ʃG�t�F�N�g�I��
	DEF_CALL	PUT_BTOWER_SCORE,	PutBattleTowerScoreWindow
	DEF_CALL	WEATHER_CHG_WAIT,	ScrWeatherChangeWait	@�V��ω��҂�

	DEF_CALL	GET_CARD_TR_NAME,	ScrBattleCardENameGet @ �J�[�he�g���[�i�[���O�擾

	DEF_CALL	GET_SLOT_ATARI,		ScrSlotAtariGet		@ �X���b�g�������擾
	DEF_CALL	GET_HERO_SITE,		GetHeroSite			@ ���@�̕������擾

	DEF_CALL	KEY_1_CHECK,		Key1Check			@ ���ʑD�J�M�P�擾�`�F�b�N
	DEF_CALL	KEY_2_CHECK,		Key2Check			@ ���ʑD�J�M�Q�擾�`�F�b�N
	DEF_CALL	KEY_4_CHECK,		Key4Check			@ ���ʑD�J�M�S�擾�`�F�b�N
	DEF_CALL	KEY_6_CHECK,		Key6Check			@ ���ʑD�J�M�U�擾�`�F�b�N

	DEF_CALL	CHK_GANBA_RIBBON,	ChkGanbaRibbon		@ �u����΃��{���v�������Ă��邩
	DEF_CALL	SET_GANBA_RIBBON,	SetGanbaRibbon		@ �u����΃��{���v���Z�b�g����
	DEF_CALL	CHK_GANBA_RIBBON_PARA_EXP,ChkGanbaRibbonParaExp	@ �u����΃��{���v��Ⴆ�邩
	DEF_CALL	COMM_SET_TALK_OBJ,		SetTalkObjSiteToHero	@���@�̕�������������
	DEF_CALL	COMM_RESET_TALK_OBJ,	ResetTalkObjSite		@����

	DEF_CALL	ADD_MOVE_POKE,		AddMvPoke			@�ړ��|�P�����Z�b�g
	DEF_CALL	CHK_KANASIDA_IWA,	ChkKanasidaIwa		@�J�i�V�_�g���l����ӂ��`�F�b�N

	DEF_CALL	TYPE_KUSA_ALL_CHECK,	TypeKusaAllCheck		@ �����|�P�������莝���ł��邩�`�F�b�N
	DEF_CALL	CONTEST_MAP_CHANGE,		AddContestMapChangeTask
	DEF_CALL	MITSURU_POKE_SET,	ScrMitsuruPokeSet	@�~�c���̕ߊl�|�P�����Z�b�g
	DEF_CALL	CHK_FIRST_POKENO,	ScrFirstPokeNumChk	@���߂Ẵ|�P�����i���o�[�`�F�b�N

	DEF_CALL	BASE_ENTRY_NAME,	MakeBaseNameScript	@ �o�^�����n�̎�����̖��O���擾

	DEF_CALL	POKE_BOX_SPACE_CHECK,	BoxSpacePokemonCheck	@ �|�P�������{�b�N�X�ɓ�����邩�`�F�b�N
	DEF_CALL	TENJI_MOVE,			AddTenjiMoveTask	@ �_���h�ꏈ��

	DEF_CALL	WRITE_NOW_FLOOR,	ElevatorNowFloorWrite	@ ���݂̃t���A����\������

	DEF_CALL	BASE_GOODS_TATE,	BaseGoodsTateCheck	@ ���̎�ނ��擾
	DEF_CALL	CHK_TEMOTI_POKERUS,	ScrTemotiPokerusCheck	@ �莝���Ƀ|�P���X�����邩�̃`�F�b�N
	DEF_CALL	SET_CRACKED_ICE,	SetAllCrackedIceCell	@���l�W���̊��ꂽ���̔��f����

	DEF_CALL	KODAI_POKE_SHAKE,	AddKodaiMoveTask	@ ���Ñ�|�P�����̗h��

	DEF_CALL	KODAI_POKE_FIGHT,	StartKodaiPokeEncountEvent	@���Ñ�|�P�����Ƒΐ�
	DEF_CALL	SP_WILD_POKE_FIGHT,	StartSpPokeEncountEvent		@���ʂȃ|�P�����Ƃ̑ΐ�
	DEF_CALL	TENJI_POKE_FIGHT,	StartTenjiPokeEncountEvent	@�_���|�P�����Ƃ̑ΐ�

	DEF_CALL	TR_STOP_OBJMOVE,	StopTrainerObjMove		@�g���[�i�[����R�[�h��Î~��Ԃ�

	DEF_CALL	TENJI_MOVE2,		AddTenjiMove2Task	@ �_���h�ꏈ���Q

	DEF_CALL	CHK_HIDE_MEGANE,	ChkHideKuroiMegane	@ �B���́u���낢���K�l�v���肵�Ă��邩
	DEF_CALL	HIDERI_FLASH		HideriFlashEffect	@ �͂��߂āu�Ђł�v�ɂȂ������̉�ʃt���b�V���G�t�F�N�g
	DEF_CALL	CHANGE_MAP,			AddMapChangeSeNotTask	@�}�b�v�J�ڌĂяo��
	DEF_CALL	CHANGE_MAP_FALL,	AddMapChangeFallTask	@���Ƃ����G�t�F�N�g�̃}�b�v�J�ڌĂяo��

	DEF_CALL	OPEN_CONPOKE_CG,	ContestPokeCGWinOpen	@�R���e�X�g�ł̃|�P����CG�\���Z�b�g
	DEF_CALL	CLOSE_CONPOKE_CG,	ContestPokeCGWinClose	@���\����del����
	DEF_CALL	CARD_E_FOBJ_SET,	BattleCardEFObjSet		@�J�[�h���ΐ펞�̃t�B�[���hOBJ���Z�b�g����
	DEF_CALL	KOKORO_POKE_FIGHT,	StartKokoroPokeEncountEvent	@��̌Ǔ��ł̃|�P�����ΐ�
	DEF_CALL	SET_WEATHER_R119,	SetWeatherR119			@���[�h�P�P�X�̓V�󑀍�
	DEF_CALL	SET_WEATHER_R123,	SetWeatherR123			@���[�h�P�Q�R�̓V�󑀍�

	DEF_CALL	SET_CON_MUS,		ContestMusSet			@�R���e�X�g
	DEF_CALL	GET_LISTNO_POKENO,	GetListNoPokeNo			@���X�g����I�񂾔ԍ��̃|�P�����i���o�[�擾

	DEF_CALL	WAZA_WASURE_TAMAGO	WazaWasureTamagoCheck	@ �Z�Y�ꂶ������^�}�S�`�F�b�N
	DEF_CALL	ADD_TOWER_OWNER		AddTowerOwner			@ �o�g���^���[�I�[�i�[�ǉ�����

	DEF_CALL	BASE_NEW_MAKE_MOVE	MyBaseNewMakeMove		@ �V�K��n�쐬�̂��߂̈��z������
	DEF_CALL	PUSH_TEMOTI_ITEM,	BackUpTemotiItem		@ �莝��������e��ޔ�

	DEF_CALL	BGM_FADEOUT_END,	FieldBGMFadeOutEnd		@ BGM�t�F�[�h�A�E�g���I��

	DEF_CALL	SET_GAME_DAY,		ScrSetWazaGetGameDays	@ �A�C�e������肵�����ɂ����Z�b�g
	DEF_CALL	GET_ONE_WEEK,		ScrGetOneWeekFromWazaGet@ ������T�Ԉȏソ���Ă��邩

	DEF_CALL	CHK_HYOUSYOU_NUM,	ZukanHyousyouCheckHouen @ �z�E�G���}�ӂ̕\���󂪌���邩

	DEF_CALL	OYANAME_CHK,		ScrOyaNameCmp			@ �e����r
	DEF_CALL	GET_LOT_NUM_MSG,	ScrPokeLotNumGet		@ ������i���o�[�̕�����擾

	DEF_CALL	TV_TOWER_HUSEI_CHK,	TVBattleTowerHuseiChk	@ �C���^�r���A�[���폜�A�V�[���N���A

	DEF_CALL	TRADE_DISABLE_ITEM_CHK, ScrCheckTradeDisableItem	@�莝���|�P�����̂ǂ����`�F�b�N

	DEF_CALL	GET_CONT_RND,		GetContRnd			@�R���e�X�g�p�̃����_���l�擾

	EV_CALL_LABEL	ScriptCallTableEnd




