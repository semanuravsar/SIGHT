from objects.transceiver import TransceiverUnit

class Communicator_1:

    INSTRUCTIONS = {   
        "do-nothing":{
            "instruction_name":"do-nothing",
            "mode_no":0,
            "steps": ["do_nothing"]
        },    
        "ping":{
            "instruction_name":"ping",
            "mode_no":1,
            "steps": ["pinging", "dead_timing_before_waiting_for_reply", "waiting_for_reply", "dead_timing_before_message", "sending_message"],
            "dead_timing_before_waiting_for_reply":0.005, # 5ms
            "dead_timing_before_message":0.005, # 5ms
            "ping_duration":0.005,
            "wait_for_reply_duration":0.02
        },      
        "listen":{
            "instruction_name":"listen",
            "mode_no":2,
            "steps": ["listening", "deadtiming_before_replying", "replying"]
        },                 
    }

    def __init__(self, transceiver:TransceiverUnit=None):       
        self.TRANSCEIVER = transceiver
        self.NUMBER_OF_TRANSMITTERS = transceiver.get_number_of_transmitters()

        self.last_time_instruction_changed = 0
        self.instruction = Communicator_1.INSTRUCTIONS["do-nothing"]
        self.instruction_step = self.instruction["steps"][0]

        self.TRANSCEIVER.turn_off_all_transmitters()
        self.receiver_states = None   
        self.simulation_time = 0

    def __ping(self):
        print(self.instruction_step)
        if self.instruction_step == "pinging":
            self.TRANSCEIVER.turn_on_all_transmitters()
            ping_duration = self.instruction["ping_duration"]
            if self.simulation_time - self.last_time_instruction_changed > ping_duration:
                self.instruction_step = "dead_timing_before_waiting_for_reply"                
                self.last_time_instruction_changed = self.simulation_time

        elif self.instruction_step == "dead_timing_before_waiting_for_reply":
            self.TRANSCEIVER.turn_off_all_transmitters()
            dead_timing_before_waiting_for_reply = self.instruction["dead_timing_before_waiting_for_reply"]
            if self.simulation_time - self.last_time_instruction_changed > dead_timing_before_waiting_for_reply:
                self.instruction_step = "waiting_for_reply"
                self.last_time_instruction_changed = self.simulation_time                

        elif self.instruction_step == "waiting_for_reply":
            self.TRANSCEIVER.turn_off_all_transmitters()
            wait_for_reply_duration = self.instruction["wait_for_reply_duration"]
            if self.simulation_time - self.last_time_instruction_changed > wait_for_reply_duration:
                self.instruction_step = "pinging"
                self.last_time_instruction_changed = self.simulation_time
                self.instruction = Communicator_1.INSTRUCTIONS["do-nothing"]
        
    def run_communicator(self, simulation_time:float):

        self.TRANSCEIVER.update_instruction_now(self.instruction_step)
        self.simulation_time = simulation_time
        if self.instruction["instruction_name"] == "do-nothing":
            self.instruction = Communicator_1.INSTRUCTIONS["ping"]
            self.instruction_step = self.instruction["steps"][0]
        if self.instruction["instruction_name"] == "ping":
            self.__ping()

        #update simulation

        # if 
     
        # self.TRANSCEIVER.turn_on_transmitter((self.counter//10)%self.NUMBER_OF_TRANSMITTERS)  
        # self.counter += 1
        # self.receiver_states = self.transceiver.get_receiver_states()

        pass

        



         

        

