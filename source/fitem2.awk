BEGIN{

	count = 0

	#=================================================================
	#
	#�t�B�[���h��̃A�C�e���̃��x������
	#
	#=================================================================
	printf( "//�t�B�[���h��̃A�C�e���̃t���O \n" )
	print ""
	print ""
	print "#define FV_BALL_START	(FV_FLAG_START+300)"
}


NF >= 6{
	if( NR > 4 )
	{
		print "#define FV_BALL1_" $7 " (FV_BALL_START+" $7 ")"
	}
}



