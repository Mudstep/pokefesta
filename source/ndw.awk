#
#	�e�[�u����`�����X�N���v�g
#	2000.12.5 by tama
#
#	�g�p���邳���̏���
#	awk -f ndw.awk ���̓t�@�C����(�g���q��.ndw�ȊO�F�߂Ȃ�)
#
#�@���͂���t�@�C����
#		�c
#	
#

BEGIN{
	start_flag = 0
	}

$0 ~/\/\*[ |	]*NDW TABLE START/{
	start_flag = 1
#	printf("enum{\n");
	counter = 0;
	}

(start_flag ==1) && $0 ~/\/\*[ |	]*NDW TABLE END/{
	start_flag = 0
#	printf("};\n\n");
	}

(start_flag ==1) && $0 ~/,[ |	]*\/\*[ |	]*/{
	now = $0
	sub(/.*\/\*[ |	]*/,"",now)
	sub(/[ |	]*\*\/.*$/,"",now)
	printf("#define %-20s\t %3d\n",now,counter);
#	printf("\t%-20s\t//%03d\n",now ",",counter)
	counter++;
	}


