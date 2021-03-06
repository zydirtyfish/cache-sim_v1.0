//////////////////////////////////////////////////////////////////////////
// cache-sim for block level cache test
// Author: zydirtyfish
// Date: 2017-08

#include "Config.h"
#include "cache-sim.h"
#include "run.h"

void init_cache(struct cache_c *ctx,const char *config_file);
void destroy_cache(struct cache_c *ctx);

int main(int argc, char *argv[])
{
	cache_c *ctx;
	char config_file[500];
	//strcpy(config_file,"cache-sim_v1.0/config");

	strcpy(config_file,argv[1]);

	ctx = (struct cache_c *)malloc(sizeof(struct cache_c));
	init_cache(ctx,config_file);
	
	if(argc >= 3)
		ctx->algorithm_type = atoi(argv[2]); 
	if(argc >= 4)
		ctx->block_num_conf = strtoull(argv[3],NULL,10);
	if(argc >= 5)
		ctx->PARA = atoi(argv[4]);
	if(argc >= 6)
		ctx->K = atof(argv[5]);
	
	/*开辟缓存空间*/
	if(ctx->algorithm_type != ARC && ctx->algorithm_type != LEA)
		ctx->cache_blk = (struct list_entry*)malloc(sizeof(struct list_entry)*ctx->block_num_conf);
	/*创建统计对象*/
	ctx->stat = new Stat();
	
	RUN *run = new RUN(ctx);
	run->exec(ctx);
	run->show_result(ctx);
	delete run;
	destroy_cache(ctx);
	return 0;
}

void init_cache(struct cache_c *ctx,const char *config_file)
{
	/*根据配置文件配置缓存参数*/
	Config *cf = new Config();
	char tmp[LINE_LENGTH];

	cf->Get(config_file,"trace_type",tmp);
	ctx->trace_type=atoi(tmp);

	cf->Get(config_file,"algorithm_type",tmp);
	ctx->algorithm_type = atoi(tmp);

	cf->Get(config_file,"block_num_conf",tmp);
	ctx->block_num_conf = strtoull(tmp,NULL,10); /*最后一个参数表示10进制*/

	cf->Get(config_file,"block_size_conf",tmp);
	ctx->block_size_conf = atoi(tmp);

	cf->Get(config_file,"write_algorithm_conf",tmp);
	ctx->write_algorithm_conf = atoi(tmp);

	cf->Get(config_file,"log_start",tmp);
	ctx->log_start = atoi(tmp);

	cf->Get(config_file,"log_num",tmp);
	ctx->log_num = atoi(tmp);
	
	cf->Get(config_file,"log_prefix",tmp);
	strcpy(ctx->log_prefix,tmp);

	cf->Get(config_file,"out_prefix",tmp);
	strcpy(ctx->out_prefix,tmp);

	cf->Get(config_file,"PARA",tmp);
	ctx->PARA=atoi(tmp);

	cf->Get(config_file,"K",tmp);
	ctx->K=atof(tmp);

	cf->Get(config_file,"hir_num_conf",tmp);
	ctx->hir_num_conf=atof(tmp);

	cf->Get(config_file,"stack_size_conf",tmp);
	ctx->stack_size_conf=atof(tmp);

	ctx->lru = ctx->mru = NULL;
	delete cf;

}

void destroy_cache(struct cache_c *ctx)
{	
	delete ctx->stat;
	if(ctx->algorithm_type != ARC && ctx->algorithm_type != LEA)
		free(ctx->cache_blk);
	free(ctx);
}
