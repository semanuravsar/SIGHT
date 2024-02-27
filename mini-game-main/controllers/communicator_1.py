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
            "steps": ["pinging", "dead_timing_before_waiting_for_reply", "waiting_for_reply", "dead_timing_before_message_sending", "sending_message"],
            "dead_timing_before_waiting_for_reply":0.005, # 5ms
            "dead_timing_before_message":0.005, # 5ms
            "ping_duration":0.005,
            "wait_for_reply_duration":0.02,
            "message_send_duration":0.05,

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
            
            "listening_duration":[0.1,1]               
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

    def __do_nothing(self):
        self.TRANSCEIVER.turn_off_all_transmitters()
        self.TRANSCEIVER.turn_off_all_transmitters()
        self.instruction = Communicator_1.INSTRUCTIONS["ping"]
        self.instruction_step = self.instruction["steps"][0]

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

            #the other robots should not send any message during this time
            receiver_states = self.TRANSCEIVER.get_receiver_states()
            for receiver_state in receiver_states:
                if receiver_state == 1:
                    self.instruction_step = "dead_timing_before_waiting_for_reply"                
                    self.last_time_instruction_changed = self.simulation_time   
                break                   

        elif self.instruction_step == "waiting_for_reply":
            self.TRANSCEIVER.turn_off_all_transmitters()
            wait_for_reply_duration = self.instruction["wait_for_reply_duration"]
            if self.simulation_time - self.last_time_instruction_changed > wait_for_reply_duration:
                self.instruction = Communicator_1.INSTRUCTIONS["do-nothing"]
                self.instruction_step = self.instruction["steps"][0]
                self.last_time_instruction_changed = self.simulation_time
                return
            
            #the other robots should send a repply ping during this time
            receiver_states = self.TRANSCEIVER.get_receiver_states()
            receiver_states[0] = 1
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


        pass

    def run_communicator(self, simulation_time:float):

        self.TRANSCEIVER.update_instruction_now(self.instruction_step)
        self.simulation_time = simulation_time
        if self.instruction["instruction_name"] == "do-nothing":            
            self.__do_nothing()
        elif self.instruction["instruction_name"] == "ping":
            self.__ping()
        elif self.instruction["instruction_name"] == "listen":
            self.__listen()


        pass

        



         

        

