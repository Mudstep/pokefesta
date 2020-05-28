//----------------------------------------------------------
//	�|�P�����̃p���[���[�^�[�\��
//----------------------------------------------------------
#ifndef __PARAM_H__
#define __PARAM_H__

#define	MONS_MALE		0
#define	MONS_FEMALE		254
#define	MONS_UNKNOWN	255

#if 0
typedef struct
{
	u16	monsno;						//02h
	u16 item;						//04h
	
	u16	waza[4];					//0ch

	u8	pp[4];						//10h

	u8	pp_count;					//11h
	u8	style;						//12h	�������悳
	u8	beautiful;					//13h	��������
	u8	cute;						//14h	���킢��
	
	u32	id_no;						//18h
	u32	exp;						//1ch

	u8	hp_exp;						//1dh
	u8	pow_exp;					//1eh
	u8	def_exp;					//1fh
	u8	agi_exp;					//20h

	u8	spepow_exp;					//21h
	u8	spedef_exp;					//22h
	u8	friend;						//23h
	u8	clever;						//24h	��������
	
	u32	pokerus		:8;				//25h
	u32	get_place	:8;				//28h	�߂܂����ꏊ
	u32	get_level	:7;				//28h	�߂܂������x���i�{����8�r�b�g������Ȃ����ǁj
	u32	get_cassette:4;				//28h	�߂܂����J�Z�b�g�i�J���[�o�[�W�����j
	u32	get_ball	:4;				//28h	�߂܂����{�[��
	u32	oyasex		:1;				//28h	�e�̐���
	
	u32	hp_rnd		:5;				//2ch
	u32	pow_rnd		:5;				//2ch
	u32	def_rnd		:5;				//2ch
	u32	agi_rnd		:5;				//2ch
	u32	spepow_rnd	:5;				//2ch
	u32	spedef_rnd	:5;				//2ch
	u32	tamago_flag	:1;				//2ch	0:�^�}�S����Ȃ�1:�^�}�S����
	u32	speabi		:1;				//2ch	����\�́i0:����\��1�@1:����\��2�j

	u16	checksum;					//2eh
	u8	strong;						//2fh	�����܂���
	u8	fur;						//30h	�щ�

	u32	personal_rnd;				//34h

	u8	nickname[MONS_NAME_SIZE+EOM_SIZE];				//3fh
	u8	mark;						//40h	�|�P�����ɂ���}�[�N�i�{�b�N�X�j

	u8	oyaname[PERSON_NAME_SIZE+EOM_SIZE];				//48h

	u32	stylemedal			:3;		//4ch	�������悳�M��
	u32	beautifulmedal		:3;		//4ch	���������M��
	u32	cutemedal			:3;		//4ch	���킢���M��
	u32	clevermedal			:3;		//4ch	���������M��
	u32	strongmedal			:3;		//4ch	�����܂����M��
	u32	champ_ribbon		:1;		//4ch	�`�����v���{��
	u32	winning_ribbon		:1;		//4ch	�E�B�j���O���{��
	u32	victory_ribbon		:1;		//4ch	�r�N�g���[���{��
	u32	bromide_ribbon		:1;		//4ch	�u���}�C�h���{��
	u32	ganba_ribbon		:1;		//4ch	����΃��{��
	u32	marine_ribbon		:1;		//4ch	�}�������{��
	u32	land_ribbon			:1;		//4ch	�����h���{��
	u32	sky_ribbon			:1;		//4ch	�X�J�C���{��
	u32	country_ribbon		:1;		//4ch	�J���g���[���{��
	u32	national_ribbon		:1;		//4ch	�i�V���i�����{��
	u32	earth_ribbon		:1;		//4ch	�A�[�X���{��
	u32	world_ribbon		:1;		//4ch	���[���h���{��
	u32  :5;

	u8	amari[4];					//50h	���҂̂��߂̗]��̈�
}PokemonPasoParam;
#else

typedef struct
{
	u16	monsno;
	u16 item;
	u32	exp;
	u8	pp_count;
	u8	friend;
	u16	amari;
}PokemonPara1;

typedef struct
{
	u16	waza[4];
	u8	pp[4];								//12�o�C�g
}PokemonPara2;

typedef struct
{
	u8	hp_exp;
	u8	pow_exp;
	u8	def_exp;
	u8	agi_exp;
	u8	spepow_exp;
	u8	spedef_exp;
	u8	style;						//	�������悳
	u8	beautiful;					//	��������
	u8	cute;						//	���킢��
	u8	clever;						//�@��������
	u8	strong;						//	�����܂���
	u8	fur;						//	�щ�	12�o�C�g
}PokemonPara3;
	
typedef struct
{
	u32	pokerus		:8;
	u32	get_place	:8;				//	�߂܂����ꏊ
	u32	get_level	:7;				//	�߂܂������x��
	u32	get_cassette:4;				//	�߂܂����J�Z�b�g�i�J���[�o�[�W�����j
	u32	get_ball	:4;				//	�߂܂����{�[��
	u32	oyasex		:1;				//	�e�̐���
	
	u32	hp_rnd		:5;
	u32	pow_rnd		:5;
	u32	def_rnd		:5;
	u32	agi_rnd		:5;
	u32	spepow_rnd	:5;
	u32	spedef_rnd	:5;
	u32	tamago_flag	:1;				//	0:�^�}�S����Ȃ�1:�^�}�S����
	u32	speabi		:1;				//	����\�́i0:����\��1�@1:����\��2�j

	u32	stylemedal			:3;		//	�������悳�M��
	u32	beautifulmedal		:3;		//	���������M��
	u32	cutemedal			:3;		//	���킢���M��
	u32	clevermedal			:3;		//	���������M��
	u32	strongmedal			:3;		//	�����܂����M��
	u32	champ_ribbon		:1;		//	�`�����v���{��
	u32	winning_ribbon		:1;		//	�E�B�j���O���{��
	u32	victory_ribbon		:1;		//	�r�N�g���[���{��
	u32	bromide_ribbon		:1;		//	�u���}�C�h���{��
	u32	ganba_ribbon		:1;		//	����΃��{��
	u32	marine_ribbon		:1;		//	�}�������{��
	u32	land_ribbon			:1;		//	�����h���{��
	u32	sky_ribbon			:1;		//	�X�J�C���{��
	u32	country_ribbon		:1;		//	�J���g���[���{��
	u32	national_ribbon		:1;		//	�i�V���i�����{��
	u32	earth_ribbon		:1;		//	�A�[�X���{��
	u32	world_ribbon		:1;		//	���[���h���{��
	u32	amari_ribbon		:5;		//	���܂��Ă܂�12�o�C�g
}PokemonPara4;

typedef struct
{
	PokemonPara1	PP1;
	PokemonPara2	PP2;
	PokemonPara3	PP3;
	PokemonPara4	PP4;
}PokemonParaData11;

typedef struct
{
	PokemonPara1	PP1;
	PokemonPara2	PP2;
	PokemonPara4	PP4;
	PokemonPara3	PP3;
}PokemonParaData12;

typedef struct
{
	PokemonPara1	PP1;
	PokemonPara3	PP3;
	PokemonPara2	PP2;
	PokemonPara4	PP4;
}PokemonParaData13;

typedef struct
{
	PokemonPara1	PP1;
	PokemonPara3	PP3;
	PokemonPara4	PP4;
	PokemonPara2	PP2;
}PokemonParaData14;

typedef struct
{
	PokemonPara1	PP1;
	PokemonPara4	PP4;
	PokemonPara2	PP2;
	PokemonPara3	PP3;
}PokemonParaData15;

typedef struct
{
	PokemonPara1	PP1;
	PokemonPara4	PP4;
	PokemonPara3	PP3;
	PokemonPara2	PP2;
}PokemonParaData16;

typedef struct
{
	PokemonPara2	PP2;
	PokemonPara1	PP1;
	PokemonPara3	PP3;
	PokemonPara4	PP4;
}PokemonParaData21;

typedef struct
{
	PokemonPara2	PP2;
	PokemonPara1	PP1;
	PokemonPara4	PP4;
	PokemonPara3	PP3;
}PokemonParaData22;

typedef struct
{
	PokemonPara2	PP2;
	PokemonPara3	PP3;
	PokemonPara1	PP1;
	PokemonPara4	PP4;
}PokemonParaData23;

typedef struct
{
	PokemonPara2	PP2;
	PokemonPara3	PP3;
	PokemonPara4	PP4;
	PokemonPara1	PP1;
}PokemonParaData24;

typedef struct
{
	PokemonPara2	PP2;
	PokemonPara4	PP4;
	PokemonPara1	PP1;
	PokemonPara3	PP3;
}PokemonParaData25;

typedef struct
{
	PokemonPara2	PP2;
	PokemonPara4	PP4;
	PokemonPara3	PP3;
	PokemonPara1	PP1;
}PokemonParaData26;

typedef struct
{
	PokemonPara3	PP3;
	PokemonPara1	PP1;
	PokemonPara2	PP2;
	PokemonPara4	PP4;
}PokemonParaData31;

typedef struct
{
	PokemonPara3	PP3;
	PokemonPara1	PP1;
	PokemonPara4	PP4;
	PokemonPara2	PP2;
}PokemonParaData32;

typedef struct
{
	PokemonPara3	PP3;
	PokemonPara2	PP2;
	PokemonPara1	PP1;
	PokemonPara4	PP4;
}PokemonParaData33;

typedef struct
{
	PokemonPara3	PP3;
	PokemonPara2	PP2;
	PokemonPara4	PP4;
	PokemonPara1	PP1;
}PokemonParaData34;

typedef struct
{
	PokemonPara3	PP3;
	PokemonPara4	PP4;
	PokemonPara1	PP1;
	PokemonPara2	PP2;
}PokemonParaData35;

typedef struct
{
	PokemonPara3	PP3;
	PokemonPara4	PP4;
	PokemonPara2	PP2;
	PokemonPara1	PP1;
}PokemonParaData36;

typedef struct
{
	PokemonPara4	PP4;
	PokemonPara1	PP1;
	PokemonPara2	PP2;
	PokemonPara3	PP3;
}PokemonParaData41;

typedef struct
{
	PokemonPara4	PP4;
	PokemonPara1	PP1;
	PokemonPara3	PP3;
	PokemonPara2	PP2;
}PokemonParaData42;

typedef struct
{
	PokemonPara4	PP4;
	PokemonPara2	PP2;
	PokemonPara1	PP1;
	PokemonPara3	PP3;
}PokemonParaData43;

typedef struct
{
	PokemonPara4	PP4;
	PokemonPara2	PP2;
	PokemonPara3	PP3;
	PokemonPara1	PP1;
}PokemonParaData44;

typedef struct
{
	PokemonPara4	PP4;
	PokemonPara3	PP3;
	PokemonPara1	PP1;
	PokemonPara2	PP2;
}PokemonParaData45;

typedef struct
{
	PokemonPara4	PP4;
	PokemonPara3	PP3;
	PokemonPara2	PP2;
	PokemonPara1	PP1;
}PokemonParaData46;

typedef struct
{
	u32	personal_rnd;
	u32	id_no;
	u8	nickname[MONS_NAME_SIZE];			//M_N_S=10
	u8	country_code;						//���R�[�h
	u8	fusei_tamago_flag	:1;				//�f�[�^�����񎞂̃^�}�S�t���O
	u8	poke_exist_flag		:1;				//�|�P�������݃t���O
	u8	egg_exist_flag		:1;				//�^�}�S�Ƃ��Ă̑��݃t���O
	u8						:5;
	u8	oyaname[PERSON_NAME_SIZE];			//P_N_S=7
	u8	mark;								//�|�P�����ɂ���}�[�N�i�{�b�N�X�j
	u16	checksum;
	u16	amari;								//	���҂̂��߂̗]��̈�
											//32�o�C�g
	u8	paradata[sizeof(PokemonPara1)+
				 sizeof(PokemonPara2)+
				 sizeof(PokemonPara3)+
				 sizeof(PokemonPara4)];
}PokemonPasoParam;
#endif

typedef struct
{
	PokemonPasoParam	PPP;
	u32	condition;		//04h
	u8	level;			//05h
	u8	mailid;			//06h
	u16	hp;				//08h
	u16	hpmax;			//0ah
	u16	pow;			//0ch
	u16	def;			//0eh
	u16	agi;			//10h
	u16	spepow;			//12h
	u16	spedef;			//14h
}PokemonParam;

typedef struct
{
	u16	monsno;					//02h
	u16	pow;					//04h

	u16	def;					//06h
	u16	agi;					//08h

	u16	spepow;					//0ah
	u16	spedef;					//0ch

	u16	waza[4];				//14h
	
	u32	hp_rnd		:5;
	u32	pow_rnd		:5;
	u32	def_rnd		:5;
	u32	agi_rnd		:5;
	u32	spepow_rnd	:5;
	u32	spedef_rnd	:5;
	u32	tamago_flag	:1;			//18h	0:�^�}�S����Ȃ�1:�^�}�S����
	u32	speabi		:1;			//18h	����\�́i0:����\��1�@1:����\��2�j

	s8	abiritycnt[8];			//20h

	u8	speabino;				//21h
	u8	type1;					//22h
	u8	type2;					//23h
	u8	work;					//24h	4�o�C�g���E�␳
										//���������͂ւ񂵂�p�p�����[�^
										//�ړ��͌���
										
	u8	pp[4];					//28h

	u16	hp;						//2ah
	u8	level;					//2bh
	u8	friend;					//2ch

	u16	hpmax;					//2eh
	u16 item;					//30h

	u8	nickname[MONS_NAME_SIZE+EOM_SIZE];			//3bh
	u8	pp_count;				//3ch

	u8	oyaname[PERSON_NAME_SIZE+EOM_SIZE];			//44h

	u32	exp;					//48h
	u32	personal_rnd;			//4ch
	u32	condition;				//50h
	u32	condition2;				//54h
	u32	id_no;					//58h

}PokemonServerParam;

typedef struct
{
	u16	waza[4];				//08h
	u8	pp[4];					//0ch
	u8	pp_count;				//10h
}PokemonWazaParam;

typedef	struct
{
	u8	battleeffect;
	u8	damage;
	u8	wazatype;
	u8	hitprobability;

	u8	pp;
	u8	addeffect;
	u8	attackrange;
	s8	attackpri;

	u8	flag;
}WazaTblDat;

typedef	struct
{
	u8	loopcnt:7;
	u8	flag:1;
}WazaEffectWork;

typedef	struct
{
	u32	henshin_rnd;			//�ւ񂵂�p�̌�����

	u16	kanashibariwazano;		//���Ȃ��΂�Z�i���o�[
	u16	encorewazano;			//�A���R�[���Z�i���o�[

	u8	successcnt;
	u8	takuwaerucnt;			//�����킦��J�E���^�[
	u8	migawarihp;				//�݂����q�b�g�|�C���g
	u8	kanashibaricnt		:4;	//���Ȃ��΂�J�E���^�[
	u8	kanashibaricnttmp	:4;	//���Ȃ��΂�J�E���^�[�i�����l�j

	u8	encorewazapos;			//�A���R�[���Z�|�W�V����
	u8	encoreclient;			//�A���R�[���Z�𓖂Ă�N���C�A���g
	u8	encorecnt			:4;	//�A���R�[���J�E���^�[
	u8	encorecnttmp		:4;	//�A���R�[���J�E���^�[
	u8	horobinoutacnt		:4;	//�ق�т̂����J�E���^
	u8	horobinoutacnttmp	:4;	//�ق�т̂����J�E���^

	u8	renzokugiricnt;			//��񂼂�����J�E���^
	u8	korogarucnt			:4;	//���낪��J�E���^
	u8	korogarucnttmp		:4;	//���낪��J�E���^
	u8	juudencnt			:4;	//���イ�ł�J�E���^
	u8	juudencnttmp		:4;	//���イ�ł�J�E���^
	u8	chouhatsucnt		:4;	//���傤�͂J�E���^
	u8	chouhatsucnttmp		:4;	//���傤�͂J�E���^

	u8	manazashiclientno;		//���낢�܂Ȃ����N���C�A���g
	u8	lockonclientno;			//���b�N�I�������N���C�A���g
	u8	nekodamashicnt;			//�l�R���܂��J�E���^
	u8	shimewazano;			//���ߌn�̋Z�i���o�[

	u8	namakebit			:1;	//�����Ȃ܂��p�̃t���O
	u8						:3;
	u8	monomanebit			:4; //���̂܂˂����Z�̈ʒu�r�b�g

	u8	handoucnt;				//�͂�ǂ��J�E���^
}WazaKoukaCount;

typedef	struct
{
	u8	butsuriguardcnt;
	u8	tokusyuguardcnt;
	u8	shiroikiricnt;
	u8	kyouwazaclient;

	u16	kyouwazano;
	u8	makibishicnt;
	u8	shinpicnt;

	u8	konoyubitomareflag;		//���̂�тƂ܂�
	u8	konoyubitomareno;		//���̂�тƂ܂�
}SideConditionCount;

typedef struct
{
	u32	guardflag			:1;		//�܂���A�݂���
	u32	koraeruflag			:1;		//���炦��
	u32	waruagakiflag		:1;		//��邠����
	u32	tedasukeflag		:1;		//�Ă�����
	u32	magiccortflag		:1;		//�}�W�b�N�R�[�g
	u32	yokodoriflag		:1;		//�悱�ǂ�
	u32	michidureflag		:1;		//�݂��Â�
	u32	mahiflag			:1;		//�܂ЂōU���ł��Ȃ�����
	u32	konranflag 			:1;		//������Ŏ������U��
	u32	koukanaiflag		:1;		//���������Ȃ�����
	u32	tamewazaflag		:1;		//���ߌn�̋Z�̂��߃^�[��
	u32	itemescapeflag		:2;		//�A�C�e���œ�����
	u32	huuinflag			:1;		//�ӂ�����ŋZ���o���Ȃ�����
	u32	meromeroflag		:1;		//���������ŋZ���o���Ȃ�����
	u32	kanashibariflag		:1;		//���Ȃ��΂�ŋZ���o���Ȃ�����
	u32	chouhatsuflag		:1;		//���傤�͂ŋZ���o���Ȃ�����
	u32	ichamonflag			:1;		//���������ŋZ���o���Ȃ�����
	u32	hirumuflag			:1;		//�Ђ��ŋZ���o���Ȃ�����
	u32	ppdecflag			:1;		//PP_DEC��ʂ���
	u32						:12;
	s32	butsurihitdamage;			//�J�E���^�[�p�̕����U���_���[�W�ۑ����[�N
	s32	tokusyuhitdamage;			//�~���[�R�[�g�p�̓���U���_���[�W���[�N
	u8	butsurihitclient;			//�����U��������ClientNo�ۑ����[�N�i�J�E���^�[�p�j
	u8	tokusyuhitclient;			//����U��������ClientNo�ۑ����[�N�i�~���[�R�[�g�p�j
}OneTurnFlag;					//���̃^�[�������ێ�����t���O�Q

typedef struct{
	u32	clearbodyflag		:1;		//�N���A�{�f�B����
	u32	hiraishinflag		:1;		//�Ђ炢����ŋZ��������
	u32	vanishoffflag		:1;		//�o�j�b�V���I�t�t���O
	u32	ikakuflag			:1;		//����������
	u32	traceflag			:1;		//�g���[�X����
	u32	nopressureflag		:1;		//�v���b�V���[�`�F�b�N�����Ȃ��t���O
	u32	reshuffleflag		:1;		//����ւ����t���O
	u32	itemkoraeruflag		:1;		//�A�C�e���ł��炦��
	u32						:24;
	s32	kaigaradamage;				//��������̂����p�̃_���[�W���[�N
	s32	butsurihitdamage;			//�J�E���^�[�p�̕����U���_���[�W�ۑ����[�N
	s32	tokusyuhitdamage;			//�~���[�R�[�g�p�̓���U���_���[�W���[�N
	u8	butsurihitclient;			//�����U��������ClientNo�ۑ����[�N�i�J�E���^�[�p�j
	u8	tokusyuhitclient;			//����U��������ClientNo�ۑ����[�N�i�~���[�R�[�g�p�j
}OneSelfTurnFlag;				//�����̃^�[�������ێ�����t���O�Q

typedef	struct
{
	u8	miraiyochicnt[4];
	u8	miraiyochiattackclient[4];
	s32	miraiyochidamage[4];
	u16	miraiyochiwazano[4];
	u8	negaigotocnt[4];			//�˂������ƃJ�E���^
	u8	negaigotoselmonsno[4];		//�˂������ƃZ�������X
	u8	FCCcnt;
	u8	hatakiotosuitem[2];		//�͂������Ƃ��A�C�e��
}FieldConditionCount;

typedef	struct
{
	u16		pow;			//�Z�b�g����p���[����(u8��OK������4�o�C�g���E�΍�j
	u16		level;			//�Z�b�g����|�P�����̃��x��
	u16		monsno;			//�Z�b�g����|�P����
}TrainerTypeNormalData;

typedef	struct
{
	u16		pow;			//�Z�b�g����p���[����(u8��OK������4�o�C�g���E�΍�j
	u16		level;			//�Z�b�g����|�P�����̃��x��
	u16		monsno;			//�Z�b�g����|�P����
	u16		waza[4];		//�����Ă�Z
}TrainerTypeWazaData;

typedef	struct
{
	u16		pow;			//�Z�b�g����p���[����(u8��OK������4�o�C�g���E�΍�j
	u16		level;			//�Z�b�g����|�P�����̃��x��
	u16		monsno;			//�Z�b�g����|�P����
	u16		itemno;			//�Z�b�g����A�C�e��
}TrainerTypeItemData;

typedef	struct
{
	u16		pow;			//�Z�b�g����p���[����(u8��OK������4�o�C�g���E�΍�j
	u16		level;			//�Z�b�g����|�P�����̃��x��
	u16		monsno;			//�Z�b�g����|�P����
	u16		itemno;			//�Z�b�g����A�C�e��
	u16		waza[4];		//�����Ă�Z
}TrainerTypeMultiData;

typedef	struct
{
	u8		datatype;
	u8		type;
	u8		sex;
	u8		chrnum;
	u8		name[6];
	u16		item[4];
	u8		fighttype;		//�퓬�̎�ށi1��1��2��2���H�j
	u32		aitbl;
	u8		monscount;		//�����Ă�|�P�����̐�
	void	*TPD;
}TrainerData;

typedef	struct
{
	u16		waza1;			//���͋Z�����Z�P
	u16		waza2;			//���͋Z�����Z�Q
	u16		wazano;			//���͋Z�i���o�[
}KyouryokuWaza;

enum{
	THROW_HYPER_BALL=0,		//�n�C�p�[�{�[��
	THROW_SUPER_BALL,		//�X�[�p�[�{�[��
	THROW_MONS_BALL,		//�����X�^�[�{�[��
	THROW_SAFARI_BALL,		//�T�t�@���{�[��
	THROW_NET_BALL,			//�l�b�g�{�[��
	THROW_DIVE_BALL,		//�_�C�u�{�[��
	THROW_NEST_BALL,		//�l�X�g�{�[��
	THROW_REPEAT_BALL,		//���s�[�g�{�[��
	THROW_TIMER_BALL,		//�^�C�}�[�{�[��
	THROW_GORGEOUS_BALL,	//�S�[�W���X�{�[��
	THROW_PREMIUM_BALL,		//�v���~�A�{�[��
	FTW_BALL_MAX,
};

typedef struct
{
	u8	MineKizetsuCount;		//�|�P�������m���ɂȂ�����
	u8	EnemyKizetsuCount;		//�퓬�œ|�����|�P�����̐�
	u8	PokeReshuffleCount;		//�������|�P��������������
	u8	ItemUseKaifukuCount;	//�A�C�e�����g�����񐔉񕜌n�i���������蓙�j

	u8	ItemUseFukkatsu;		//�A�C�e�����g�����񐔕����i���񂫂̂����܂蓙�j
	u8	DamageFlag			:1;			//�_���[�W�t���O
	u8	ThrowMasterBallFlag	:1;	//�}�X�^�[�{�[���𓊂������̃t���O
	u8						:6;
	u16	MineLastPokeNo;			//�������Ō�ɏo���Ă����|�P�����i���o�[

	u8	MineLastPokeNickname[MONS_NAME_SIZE+EOM_SIZE];
								//�������Ō�ɏo���Ă����|�P�����̃j�b�N�l�[��
	u8	FightTurn;				//�퓬�^�[����

	u8	MineLastPokeNickname2[MONS_NAME_SIZE+EOM_SIZE];
							//�������Ō�ɏo���Ă����|�P�����̃j�b�N�l�[��(2vs2)
	u8	dummy;					//4�o�C�g���E�␳

	u16	EnemyLastPokeNo;		//����̍Ō�̃|�P�����i���o�[
	u16	MineLastWazaNo;			//�������Ō�Ɏg�����Z

	u16	EnemyLastWazaNo;		//���肪�Ō�Ɏg�����Z
	u16	MineLastPokeNo2;		//�������Ō�ɏo���Ă����|�P�����i���o�[(2vs2)

	u16	GetPokeNo;				//�߂܂����|�P�����̃|�P�����i���o�[
								
	u8	GetPokeNickname[MONS_NAME_SIZE+EOM_SIZE];	//�߂܂����|�P�����ɂ����j�b�N�l�[��
	u8	dummy2;					//4�o�C�g���E�␳

	u8	ThrowBall[FTW_BALL_MAX];	
					//�퓬���Ƀ{�[���𓊂�����(�{�[���̎�ނ͏�L���Q�Ɓj
	u8	dummy3;					//4�o�C�g���E�␳
}FightTVWork;


typedef struct{
	u32	abirityflag[4];			//����\�͗p�t���O�i�N���C�A���g���p��4�o�C�g�j
}SpecialAbirityData;

typedef struct{
	u8	*PushAdrsBuf[8];
	u8	PushAdrsCnt;
}PushAdrs;

typedef struct{
	pFunc	RetServerSequence[8];
	u8		PushServerAdrsCnt;
}PushServerAdrs;

typedef struct{
	u16		wazano[4];

	u8		pp[4];

	u8		ppmax[4];

	u16		monsno;
	u8		type1;
	u8		type2;
}WazaList;

typedef struct{
	u16		hp;
	u16		dummy;
	u32		condition;
}BallGaugePara;

typedef struct{
	u8		type;			//�g���[�i�[�̎��
	u8		name[3];		//�g���[�i�[�̖��O
	u8		flag;			//��ރt���O
	u8		work;			//1�o�C�g���܂��Ă��܂��i4�o�C�g���E�␳�j
	u16		kaiwa[6];		//�ȈՉ�b
}BattleTowerTrainerData;

typedef struct{
	u16		monsno;				//�|�P�����i���o�[
	u8		item;				//�����Ă���A�C�e��
	u8		flag;				//��ރt���O
	u16		waza[4];			//�����Ă���Z
	u8		expbit;				//�w�͒l�r�b�g
	u8		chr;				//���i
	u16		work;				//2�o�C�g���܂��Ă��܂��i4�o�C�g���E�␳�j
}BattleTowerPokemonData;

typedef struct{
	u16	hpmax;
	u16	pow;
	u16	def;
	u16	agi;
	u16	spepow;
	u16	spedef;
}PushPokeParam;

typedef struct{
	u16	monsno;
	u16	item;

	u16	waza[4];

	u8	level;
	u8	pp_count;
	u8	hp_exp;
	u8	pow_exp;

	u8	def_exp;
	u8	agi_exp;
	u8	spepow_exp;
	u8	spedef_exp;

	u32	id_no;

	u32	hp_rnd		:5;
	u32	pow_rnd		:5;
	u32	def_rnd		:5;
	u32	agi_rnd		:5;
	u32	spepow_rnd	:5;
	u32	spedef_rnd	:5;
	u32	tamago_flag	:1;	// 0:�^�}�S����Ȃ�1:�^�}�S����
	u32	speabi		:1;	// ����\�́i0:����\��1�@1:����\��2�j

	u32	personal_rnd;

	u8	nickname[MONS_NAME_SIZE+EOM_SIZE];
	u8	friend;

}StockBattleTowerPokemon;

typedef struct{
	u8						level;
	u8						trainerno;
	u16						win;

	u8						name[PERSON_NAME_SIZE+EOM_SIZE];

	u8						id[4];

	u16						kaiwa[6];		//�ȈՉ�b

	StockBattleTowerPokemon	SBTP[3];

	u32						checksum;
}StockBattleTowerTrainer;

typedef struct{
	u8						level;
	u8						trainerno;
	u16						win;

	u8						name[PERSON_NAME_SIZE+EOM_SIZE];

	u8						id[4];

	u16						aisatukaiwa[6];		//�ȈՉ�b�i�������j

	u16						winkaiwa[6];		//�ȈՉ�b�i�퓬�����j

	u16						losekaiwa[6];		//�ȈՉ�b�i�퓬�����j

	StockBattleTowerPokemon	SBTP[3];

	u32						checksum;

}StockBattleCardETrainer;

typedef struct
{
	u16	MineLastPokeNo;			//�������Ō�ɏo���Ă����|�P�����i���o�[
	u16	EnemyLastPokeNo;			//���肪�Ō�ɏo���Ă����|�P�����i���o�[
	u8	TrainerName[PERSON_NAME_SIZE+EOM_SIZE];
	u8	MineLastPokeNickname[MONS_NAME_SIZE+EOM_SIZE];
								//�������Ō�ɏo���Ă����|�P�����̃j�b�N�l�[��
}StockBattleTowerTVData;

typedef struct
{
	u16	monsno;
	u16	itemno;
	u8	nickname[MONS_NAME_SIZE+EOM_SIZE];
	u8	level;
	u16	hp;
	u16	hpmax;
	u32	condition;
	u32	personal_rnd;
	u8	sex;
	u8	dummy[3];
}MultiPokeListData;

typedef struct
{
	u8 name[SEED_NAME_SIZE+EOM_SIZE];	//���O
	u8 item_equip;
	u8 item_param[18];
	u8 item_power;
}FightRamSeed;

#endif	__PARAM_H__

