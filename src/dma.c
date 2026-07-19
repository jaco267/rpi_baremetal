#include <dma.h>
#include <mem.h>
#include <mm.h>
#include <timer.h>
#include <printf.h>
//* 15 channels
dma_channel channels[15];
static u16 channel_map = 0x1F35;
//                          B A987   43  2 0
//                       0001 1111 0011 0101  
// 文檔沒有寫 哪些 dma channel 不能用  所以要去 linux 查
//* give us a channel number that is available 
static u16 allocate_channel(u32 channel) {
    if (!(channel & ~0x0F)) {  //  1111 1111.. 1111 0000  means channel smaller than 16
        if (channel_map & (1 << channel)) {  //this is available 
            channel_map &= ~(1 << channel);  // turn off channel map at that bit (set to 0) so other cant use this 
            return channel;
        }
        return -1;
    }
    //* CT_NORMAL have 6 channel   CT_NORMAL = 0x81
    u16 i = channel == CT_NORMAL ? 6 : 12;
    //* loop through 6 channel   6543210   opened in map is 4320
    for (; i >= 0; i--) {
        if (channel_map & (1 << i)) {
            channel_map &= ~(1 << i);  //set that map bit to 0 because it is now used by this channel
            return i;
        }
    }
    return CT_NONE;
}
dma_channel *dma_open_channel(u32 channel) {
    u32 _channel = allocate_channel(channel);  //give us a ch number that is available
    if (_channel == CT_NONE) {printf("INVALID CHANNEL! %d\n", channel);return 0;}
    dma_channel *dma = (dma_channel *)&channels[_channel];
    dma->channel = _channel;
    //LOW_MEMORY = bottom of RAM...  Hack for now since no allocate function
    dma->block = (dma_control_block *)((LOW_MEMORY + 31) & ~31);
    // ~31 = ~(011111)=1..1100000
    dma->block->res[0] = 0;
    dma->block->res[1] = 0;
    //* enable dma channel  
    REGS_DMA_ENABLE |= (1 << dma->channel);
    timer_sleep(3);
    //* reset dma channel
    REGS_DMA(dma->channel)->control |= CS_RESET;
    //* wait for reset done  ???
    while(REGS_DMA(dma->channel)->control & CS_RESET) ;

    return dma;
}

void dma_close_channel(dma_channel *channel) {
    channel_map |= (1 << channel->channel);   //this is unused so turn that that dma map bit 
}

void dma_setup_mem_copy(dma_channel *channel, void *dest, void *src, u32 length, u32 burst_length) {
    //tell the channel what about the dma transfer information (burst length , width.... destination width...etc)
    channel->block->transfer_info = (burst_length << TI_BURST_LENGTH_SHIFT)
		| TI_SRC_WIDTH | TI_SRC_INC | TI_DEST_WIDTH | TI_DEST_INC;
    //* tell dma what is the src/dest addr for mem copy   
                               //todo careful I've add uintptr_t
    channel->block->src_addr = (u32)(uintptr_t)src;
    channel->block->dest_addr = (u32)(uintptr_t)dest;
    channel->block->transfer_length = length;
    //* we dont use 2d_stride in here 
    channel->block->mode_2d_stride = 0;
    //* we only copy once in here??   
    channel->block->next_block_addr = 0;
}
/*
//* usage  
dma_setup_mem_copy(dma,dest+start, src+start , num_bytes, 2)
dma_start(dma)
dma_wait(dma)
*/
void dma_start(dma_channel *channel) {
    // send the DMA control block addr , se we can read info from there (should call this fn after dma_setup_mem_copy)
    //todo carefull added uintptr_t
    REGS_DMA(channel->channel)->control_block_addr = BUS_ADDRESS((u32)(uintptr_t)channel->block);
    //* start dma    CS_ACTIVE so dma start and active  
    REGS_DMA(channel->channel)->control = CS_WAIT_FOR_OUTSTANDING_WRITES
					      | (DEFAULT_PANIC_PRIORITY << CS_PANIC_PRIORITY_SHIFT)
					      | (DEFAULT_PRIORITY << CS_PRIORITY_SHIFT)
					      | CS_ACTIVE;
}

bool dma_wait(dma_channel *channel) {
    // wait  dma to finish?  since its cs_active
    while(REGS_DMA(channel->channel)->control & CS_ACTIVE) ;
    //* status = false if cs_error else true  
    channel->status = REGS_DMA(channel->channel)->control & CS_ERROR ? false : true;

    return channel->status;

}
