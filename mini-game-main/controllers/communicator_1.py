import random
from objects.transceiver import TransceiverUnit

class Communicator_1:

    INSTRUCTIONS = {   
        "do-nothing":{
            "instruction_name":"do-nothing",
            "mode_no":0,
            "steps": ["randomizing_duration", "delay_before_ping"],
            "delay_duration":[0.010, 0.050]
        },    
        "ping":{
            "instruction_name":"ping",
            "mode_no":1,
            "steps": ["pinging", "dead_timing_before_waiting_for_reply", "waiting_for_reply", "dead_timing_before_message_sending", "sending_message"],
            "ping_duration":0.005,
            "dead_timing_before_waiting_for_reply":0.005, # 5ms
            "wait_for_reply_duration":0.025,
            "dead_timing_before_message":0.010, # 10ms
            "message_send_duration":0.050,

        },      
        "listen":{
            "instruction_name":"listen",
            "mode_no":2,
            "steps": [
                "randomizing_duration",
                "listening",
                "deadtiming_before_replying_ping", 
                "replying_ping", 
                "listening_the_message"],
            
            "listening_duration":[0.100,0.200],    
            "deadtiming_before_replying_ping":0.008, #8ms 
            "replying_ping_duration":0.005, 
            "listening_the_message_timeout":0.075, # 75ms       
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


        self.listen_duration = 0
        self.delay_before_ping = 0

    def __do_nothing(self):
        self.TRANSCEIVER.turn_off_all_transmitters()

        if self.instruction_step == "randomizing_duration":
            min_duration = self.instruction["delay_duration"][0]
            max_duration = self.instruction["delay_duration"][1]

            self.delay_before_ping = random.uniform(min_duration, max_duration)
            self.last_time_instruction_changed = self.simulation_time
            self.instruction_step = "delay_before_ping"
            return
        
        elif self.instruction_step == "delay_before_ping":
            if self.simulation_time - self.last_time_instruction_changed > self.delay_before_ping:
                self.instruction = Communicator_1.INSTRUCTIONS["ping"]
                self.instruction_step = self.instruction["steps"][0]

            #if the other robots send a ping during this time, the robot should immediately change its instruction to reply-ping state
            receiver_states = self.TRANSCEIVER.get_receiver_states()
            for receiver_state in receiver_states:
                if receiver_state == 1:
                    self.instruction = Communicator_1.INSTRUCTIONS["listen"]
                    self.instruction_step = "replying_ping"
                    self.last_time_instruction_changed = self.simulation_time
                    return

    def __ping(self):
        if self.instruction_step == "pinging":
            self.TRANSCEIVER.turn_on_all_transmitters()

            ping_duration = self.instruction["ping_duration"]
            if self.simulation_time - self.last_time_instruction_changed > ping_duration:
                self.instruction_step = "dead_timing_before_waiting_for_reply"                
                self.last_time_instruction_changed = self.simulation_time    
            return      
          
        elif self.instruction_step == "dead_timing_before_waiting_for_reply":
            self.TRANSCEIVER.turn_off_all_transmitters()

            dead_timing_before_waiting_for_reply = self.instruction["dead_timing_before_waiting_for_reply"]
            if self.simulation_time - self.last_time_instruction_changed > dead_timing_before_waiting_for_reply:
                self.instruction_step = "waiting_for_reply"
                self.last_time_instruction_changed = self.simulation_time
                return       
                        
        elif self.instruction_step == "waiting_for_reply":
            self.TRANSCEIVER.turn_off_all_transmitters()
            wait_for_reply_duration = self.instruction["wait_for_reply_duration"]
            if self.simulation_time - self.last_time_instruction_changed > wait_for_reply_duration:
                #no reply is received, try listening
                self.instruction = Communicator_1.INSTRUCTIONS["listen"]
                self.instruction_step = self.instruction["steps"][0]
                self.last_time_instruction_changed = self.simulation_time
                return
            
            #the other robots should send a repply ping during this time
            receiver_states = self.TRANSCEIVER.get_receiver_states()
            for receiver_state in receiver_states:           
                if receiver_state == 1:
                    self.instruction_step = "dead_timing_before_message_sending"                
                    self.last_time_instruction_changed = self.simulation_time    
                    break
        
        elif self.instruction_step == "dead_timing_before_message_sending":
            self.TRANSCEIVER.turn_off_all_transmitters()
            dead_timing_before_message = self.instruction["dead_timing_before_message"]
            if self.simulation_time - self.last_time_instruction_changed > dead_timing_before_message:
                self.instruction_step = "sending_message"
                self.last_time_instruction_changed = self.simulation_time
                return
            
        elif self.instruction_step == "sending_message":
            self.TRANSCEIVER.turn_on_all_transmitters()
            message_send_duration = self.instruction["message_send_duration"]
            if self.simulation_time - self.last_time_instruction_changed > message_send_duration:
                self.instruction = Communicator_1.INSTRUCTIONS["listen"]
                self.instruction_step = self.instruction["steps"][0]
                self.last_time_instruction_changed = self.simulation_time
                return
    
    def __listen(self):
        if self.instruction_step == "randomizing_duration":
            self.TRANSCEIVER.turn_off_all_transmitters()
            min_duration = self.instruction["listening_duration"][0]
            max_duration = self.instruction["listening_duration"][1]

            self.listen_duration = random.uniform(min_duration, max_duration)
            self.last_time_instruction_changed = self.simulation_time
            self.instruction_step = "listening"
            return
        elif self.instruction_step == "listening":
            self.TRANSCEIVER.turn_off_all_transmitters()
            if self.simulation_time - self.last_time_instruction_changed > self.listen_duration:
                self.instruction = Communicator_1.INSTRUCTIONS["do-nothing"]
                self.instruction_step = self.instruction["steps"][0]
                self.last_time_instruction_changed = self.simulation_time
                return

            #the other robots should send a ping during this time
            receiver_states = self.TRANSCEIVER.get_receiver_states()
            for receiver_state in receiver_states:           
                if receiver_state == 1:
                    self.instruction_step = "deadtiming_before_replying_ping"                
                    self.last_time_instruction_changed = self.simulation_time    
                    break
        
        elif self.instruction_step == "deadtiming_before_replying_ping":
            self.TRANSCEIVER.turn_off_all_transmitters()
            dead_timing_before_replying_ping = self.instruction["deadtiming_before_replying_ping"]
            if self.simulation_time - self.last_time_instruction_changed > dead_timing_before_replying_ping:
                self.instruction_step = "replying_ping"
                self.last_time_instruction_changed = self.simulation_time
                return
            
        elif self.instruction_step == "replying_ping":
            self.TRANSCEIVER.turn_on_all_transmitters()
            reply_duration = self.instruction["replying_ping_duration"]
            if self.simulation_time - self.last_time_instruction_changed > reply_duration:
                self.instruction_step = "listening_the_message"
                self.last_time_instruction_changed = self.simulation_time
                return
                          
        elif self.instruction_step == "listening_the_message":
            self.TRANSCEIVER.turn_off_all_transmitters()
            listening_the_message_duration = self.instruction["listening_the_message_timeout"]

            #NOTE: normally the robot should stop listening when the all 8xN bits are sampled 
            if self.simulation_time - self.last_time_instruction_changed > listening_the_message_duration:
                self.instruction = Communicator_1.INSTRUCTIONS["do-nothing"]
                self.instruction_step = self.instruction["steps"][0]
                self.last_time_instruction_changed = self.simulation_time
                return

    def run_communicator(self, simulation_time:float):

        self.TRANSCEIVER.update_instruction(self.instruction, self.instruction_step)
        
        self.simulation_time = simulation_time
        if self.instruction["instruction_name"] == "do-nothing":            
            self.__do_nothing()
        elif self.instruction["instruction_name"] == "ping":
            self.__ping()
        elif self.instruction["instruction_name"] == "listen":
            self.__listen()


        pass

        



         

        

