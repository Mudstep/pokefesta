BEGIN{
	RS="{"			#���R�[�h��؂��"{"�ɂ���	�i�ʏ�͉��s�j
	FS="\n"			#�t�B�[���h��؂�����s�ɂ���	�i�ʏ�̓X�y�[�Xor�^�u�j

	printf( "//sxy�f�[�^�́u���݂̂h�c�v��` " )
	print "\n"
}


$8 ~/MV_SEED/ && $3 ~/SEED/ && $13 ~/seed_script/{

	symbol = toupper($2)		#�啶���ɓ���
	sub(/,.*$/,"",symbol)		#","�ȍ~���폜
	sub(/^[\t ]*/,"",symbol)	#�擪�̃X�y�[�X��^�u���폜

	id = tolower($12)			#�������ɓ���
	sub(/,.*$/,"",id)			#","�ȍ~���폜

	if ( id in ID_List ) {

		#�������l���`���Ă����������ꍇ
		printf("�����l�̒�`������܂�\n") >> "/dev/stderr"
		printf("[%s] [%s] %s\n",ID_List[id],symbol,id) >> "/dev/stderr"
		printf("ERROR! %s\n",symbol)

		exit				#�r���ŏI��

	}

	ID_List[id] = symbol		#�v�f�Ƃ��ēo�^

	#��`���o��
	printf("#define ID_%-30s %s\n",symbol,id)
}

