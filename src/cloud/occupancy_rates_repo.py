from firebase_repo import FirebaseRepo
from utils import TimeUtils


class OccupancyRatesRepository(FirebaseRepo):

    __occupancy_rates_ODS_node_name = 'parkingArea'
    __occupancy_rate_node_name = 'occupancyRate'
    __unknown_occupancy_value = 'UNKNOWN'

    def __init__(self):
        FirebaseRepo.__init__(self)

    @staticmethod
    def get_area_table_name(area_id):
        return 'area_' + area_id + '_end_time'

    def refresh_occupancies(self, counts):

        now_weekday = TimeUtils.get_local_weekday()
        if now_weekday < 6:
            now_weekday = -1
        now_hour = TimeUtils.get_local_hour()
        # TODO
        now_hour = 12

        parking_areas = self.db\
            .child(OccupancyRatesRepository.__occupancy_rates_ODS_node_name)\
            .get()

        time_config_params = [(6, 'TIME_START_SATURDAY', 'TIME_END_SATURDAY'),
                              (7, 'TIME_START_SUNDAY', 'TIME_END_SUNDAY'),
                              (-1, 'TIME_START_WEEKDAYS', 'TIME_END_WEEKDAYS')]

        time_start_name, time_end_name = [(s, e) for w, s, e in time_config_params if w == now_weekday][0]
        occupancy_rates_update = {}

        for pa in parking_areas.each():
            key = pa.key()
            pa = pa.val()
            is_occupancy_rate_set = False
            properties = pa['properties']

            if properties.has_key('TYPE_OF_FINANCIAL') is False:
                pass
            elif properties['TYPE_OF_FINANCIAL'] == 'M':
                print("OK")
                if properties[time_start_name] <= now_hour < properties[time_end_name]:
                    is_occupancy_rate_set = True

            if is_occupancy_rate_set is True and pa.has_key('area_number'):
                parking_area_id = pa['area_number']
                amount_of_parked_cars = 0
                if counts.has_key(parking_area_id):
                    amount_of_parked_cars = counts[parking_area_id]

                occupancy_rate = min(1, amount_of_parked_cars / properties['NUMBER_OF_PLACES'])
            else:
                occupancy_rate = OccupancyRatesRepository.__unknown_occupancy_value

            update_key = OccupancyRatesRepository.__occupancy_rates_ODS_node_name \
                         + '/' + str(key) \
                         + '/' + OccupancyRatesRepository.__occupancy_rate_node_name
            occupancy_rates_update[update_key] = occupancy_rate

        self.db.update(occupancy_rates_update)
