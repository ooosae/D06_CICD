.PHONY = 2 3 4 5 all

all: 2 3 4 5

2:
	cd ../ && sudo gitlab-runner exec shell stage_build
3:
	cd ../ && sudo gitlab-runner exec shell stage_test_style
4:
	cd ../ && sudo gitlab-runner exec shell stage_test_integration
5:
	cd ../ && sudo gitlab-runner exec shell stage_deploy
