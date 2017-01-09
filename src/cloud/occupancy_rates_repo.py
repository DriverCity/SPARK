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
        if now_weekday < 5:
            now_weekday = -1
        now_hour = TimeUtils.get_local_hour()

        parking_areas = self.db\
            .child(OccupancyRatesRepository.__occupancy_rates_ODS_node_name)\
            .get()

        time_config_params = [(5, 'TIME_START_SATURDAY', 'TIME_END_SATURDAY'),
                              (6, 'TIME_START_SUNDAY', 'TIME_END_SUNDAY'),
                              (-1, 'TIME_START_WEEKDAYS', 'TIME_END_WEEKDAYS')]

        time_start_name, time_end_name = [(s, e) for w, s, e in time_config_params if w == now_weekday][0]

        for pa in parking_areas.each():
            key = pa.key()
            pa = pa.val()
            is_occupancy_rate_set = False
            properties = pa['properties']

            if properties['TYPE_OF_FINANCIAL'] is 'M':
                if properties[time_start_name] <= now_hour < properties[time_end_name]:
                    is_occupancy_rate_set = True

            if is_occupancy_rate_set is True:
                occupancy_rate = min(1, counts[pa['area_number']] / properties['NUMBER_OF_PLACES'])
            else:
                occupancy_rate = OccupancyRatesRepository.__unknown_occupancy_value

            self.db \
                .child(OccupancyRatesRepository.__occupancy_rates_ODS_node_name) \
                .child(key) \
                .child(OccupancyRatesRepository.__occupancy_rate_node_name) \
                .set(occupancy_rate)
