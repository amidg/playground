import datetime
import calendar

def return_year_and_month(date_str):
    return map(int, date_str.split('-'))

def get_days_in_month(date_str):
    # parse string
    year, month = return_year_and_month(date_str)
    
    # Validate year and month
    if not (1 <= month <= 12):
        raise ValueError("Month must be between 1 and 12")
    
    # get infinitely protated days in the month
    days_in_month = calendar.Calendar().itermonthdays(year, month)
    
    # filter days
    return len([day for day in days_in_month if day != 0])



def monthly_charge(month, subscription, users):
    total_cost: int = 0 # whole cents per month

    year, month_int = return_year_and_month(month)
    start_of_month = datetime.date(year, month_int, 1)
    num_days_in_month = get_days_in_month(month)
    end_of_month = datetime.date(year, month_int, num_days_in_month)

    # Iterate over each user
    for user in users:
        # Get the user's active period
        activated_on = user["activated_on"]
        deactivated_on = user["deactivated_on"]
        
        # Calculate overlap of user activity with the month
        active_start = max(start_of_month, activated_on)
        active_end = min(end_of_month, deactivated_on or end_of_month)
        
        # Ensure the user was active for at least part of the month
        if active_start <= active_end:
            active_days = (active_end - active_start).days + 1
            daily_cost = subscription["monthly_price_in_cents"] / num_days_in_month
            total_cost += int(active_days * daily_cost)
        else:
            raise ValueError("Incorrect days")
    
    return total_cost
  

if __name__ == "__main__":
    plan = {
        "id": 1,
        "customer_id": 1,
        "monthly_price_in_cents": 5_000
    }

    users = [
        {
            "id": 1,
            "name": "foo",
            "activated_on": datetime.date(2019, 1, 1),
            "deactivated_on": None,
            "customer_id": 1
        },
        {
            "id": 2,
            "name": "foo",
            "activated_on": datetime.date(2019, 1, 1),
            "deactivated_on": datetime.date(2022, 4, 15),
            "customer_id": 1
        }
    ]

    no_users = []

    month: str = "2022-04" # april 2022
    print(monthly_charge(month, plan, users))
    print(monthly_charge(month, plan, no_users))
